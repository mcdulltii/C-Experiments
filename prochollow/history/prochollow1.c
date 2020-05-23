#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

void parent(pid_t pid)
{
    int	status;
    waitpid(pid, &status, 0);

    if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        sleep(1);
        // Modify suspended hello

        ptrace(PTRACE_CONT, pid, (char*)1, 0);
    }
}

int main(int argc, char **argv)
{
    pid_t result;
	int pid_pipe[2];
	int pid;
	char *pid_buf=(char*)malloc(sizeof(char*));
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
		execv("./hello1", argv);
		free(pid_buf);
        break;
    case -1:
        printf("ERROR\n");
		break;
    default:
		close(pid_pipe[1]);
		read(pid_pipe[0], pid_buf, 100);
		pid = strtol(pid_buf, NULL, 10);
        parent(pid);
        break;
    }
}

