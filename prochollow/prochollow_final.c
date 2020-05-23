#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

typedef unsigned char BYTE;

char* maps(char*, int);
int mem(char*, char*, char*);

void parent(char* pid) {
    int status;
    int count = 1;
    char *addr[10], *start_addr, *end_addr;
    char cwd[50];
    pid_t pid_wait = strtol(pid, NULL, 10);
    // Wait for child process to run PTRACE_TRACEME
    waitpid(pid_wait, &status, 0);
    // Retrieve pwd
    if (getcwd(cwd, sizeof(cwd)) == NULL) exit(1);

    // Sighalt child process to rewrite its virtual memory
    if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {    
        // Only retrieve memory map of CODE section
        while(strstr(maps(pid, count), cwd) != NULL) {
            // Retrieve start address
            addr[count] = maps(pid, count);
            if (count == 1) start_addr = strtok(addr[count], "-");
            count++;
        }
        // Retrieve end address
        end_addr = strtok(maps(pid, count-1), " ");
        end_addr = strtok(end_addr, "-");
        end_addr = strtok(NULL, "-");

        // Rewrite child process's virtual memory
        mem(pid, start_addr, end_addr);

        ptrace(PTRACE_CONT, pid, (char*)1, 0); // Continue suspended process
        //kill(pid_wait, SIGKILL); // Kill suspended process
    }
}

int main(int argc, char* argv[]) {
    pid_t result;
    int pid_pipe[2];
    int pid;
	char *pid_buf=(char*)malloc(sizeof(char*));

    // Create pipe for transfer of pid
	if (pipe(pid_pipe)==-1) {
        puts("ERROR");
        return 1;
	}

    result = fork();
    switch (result) {
    case 0:
		close(pid_pipe[0]);
		pid = getpid();
		sprintf(pid_buf, "%d", pid);
		write(pid_pipe[1], pid_buf, strlen(pid_buf)+1);
		close(pid_pipe[1]);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
        // Runs hello process with our argv, simulating the hello process
		execv("./hello", argv);
		free(pid_buf);
        break;
    case -1:
        puts("ERROR");
		break;
    default:
		close(pid_pipe[1]);
		read(pid_pipe[0], pid_buf, 100);
        parent(pid_buf);
        break;
    }
    return 0;
}

char* maps(char* pid, int linesearch) {
    FILE *ptr;
    int line = 1, found = 0;
    char* fl, liner[500], *rtn = (char*)malloc(sizeof(char*));
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%s/maps", pid);
    ptr = fopen(fl,"r");
    if (ptr == NULL) exit(1);
    
    // Retrieves line with linesearch from /proc/{pid}/maps
    while(fgets(liner, 500, ptr)) {
        if (linesearch == line) {
            found = 1;
            break;
        }
        line++;
    }
    if (!found) exit(1);
    free(fl);
    strcpy(rtn, liner);
    return rtn;
}

int mem(char* pid, char* start_addr, char* end_addr) {
    unsigned long int s_addr = strtoul(start_addr, NULL, 16);
    unsigned long int e_addr = strtoul(end_addr, NULL, 16);
    unsigned long length = e_addr - s_addr;
    char *fl;
    int i, offset = 0;
    BYTE *buf = (BYTE*)malloc(length);
    char *buf2;
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%s/mem", pid);
    int fd = open(fl, O_RDWR);
    if (fd == -1) return 1;
    
    lseek(fd, s_addr, SEEK_SET);
    read(fd, buf, length);
    // Check bytes for byte sequence of main function in child process to obtain offset address
    for (i=0; i<length; i++) {
        if ((int)buf[i] != 0x55) continue;
        if ((int)buf[i+1] != 0x48) continue;
        if ((int)buf[i+2] != 0x89) continue;
        if ((int)buf[i+3] != 0xe5) continue;
        offset = i;
        break;
    }
    
    // Rewrite specific/all bytes i.e. 0x0a -> 0x10
    lseek(fd, s_addr+offset+47, SEEK_SET);
    buf2 = (char[]) {0x10};
    if (write(fd, buf2, 1) == -1) return 1;

    free(fl);
    free(buf);
    return 0;
}
