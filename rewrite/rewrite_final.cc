#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ptrace.h>

void sp(char*);
char* fp(unsigned long int*, unsigned long*, int*);
int mp(unsigned long int*, unsigned long*, int*, int*, int, int*, int);
int mm(unsigned long int*, unsigned long*, int*, int*, int, int*, int);
char *de(int*, int, int);
void c(char *buf, ...);
void chk(char*, char*);
void __attribute__((constructor)) flag(void);

char s[512]={0};
char *adr, *pm, *ad1, *ad2;
int out, cnt = 2;

// Check debugger
int gdb(void *add) {
    if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1) return 1;
    return 0;
}

int main(void)
{
    pid_t f;
    char *inp;
    char *ad_buf=(char*)malloc(sizeof(char*)), *leng_buf=(char*)malloc(sizeof(char*)), *pid_buf=(char*)malloc(sizeof(char*));
    unsigned long int ad;
    unsigned long leng;
    int pid;

    if (gdb()) exit(0);

    // Obfuscated strings (0x1A for first three, then 0x56)
    int hp[] = {0x75,0x82,0x7f,0x7b,0x8a,0x77,0x1a,};
    int mps[] = {0x49,0x8a,0x8c,0x89,0x7d,0x49,0x3f,0x7e,0x49,0x87,0x7b,0x8a,0x8d,0x1a,};
    int mmy[] = {0x49,0x8a,0x8c,0x89,0x7d,0x49,0x3f,0x7e,0x49,0x87,0x7f,0x87,0x1a,};
    int nd[] = {0x8b,0x89,0x87,0xbc,0xb5,0xc3,0x86,0xba,0x87,0xbc,0xcf,0x87,0xc4,0xbd,0xb5,0x8a,0x87,0x8d,0x89,0xc8,0xc4,0x8a,0x8d,0x87,0xcc,0x89,0xb5,0x89,0x8f,0x8a,0x8d,0x8b,0x8e,0x88,0x8f,0x56,};

    // Pipe for parent and child process, to transfer pid, address and length of string s
    int pip1[2];
    int pip2[2];
    int pip3[2];
    if (pipe(pip1)==-1 || pipe(pip2)==-1 || pipe(pip3)==-1) {
        puts("Incorrect");
        return 1;
    }

    // String s to be changed
    c(s, 'T','h','i','s','_','I','n','i','t','i','a','l','_','D','a','t','a','_','i','s','_','s','t','i','l','l','_','n','o','t','_','e','n','o','u','g','h','\0');
    if (s == NULL)
    {
        puts("Incorrect");
        return 1;
    }
    // Spawn child process
    f = fork();
    pid = 0;
    if (f < 0) {
        puts("Incorrect");
        return 1;
    } else if (f == 0) {
        close(pip1[1]);
        close(pip2[1]);
        close(pip3[1]);
        read(pip1[0], ad_buf, 100);
        read(pip2[0], leng_buf, 100);
        read(pip3[0], pid_buf, 100);
        
        // Convert address, length of string and pid of process
        ad = strtoul(ad_buf, NULL, 16);
        leng = strtol(leng_buf, NULL, 10);
        pid = strtol(pid_buf, NULL, 10);

        // Read /proc/{pid}/maps to find address of [heap]
        out = mp(&ad, &leng, &pid, hp, sizeof(hp)/sizeof(hp[0]), mps, sizeof(mps)/sizeof(mps[0]));
        if (out==1) {
            puts("Incorrect");
            return 1;
        }
        // Read /proc/{pid}/mem to overwrite string s within process's virtual memory
        out = mm(&ad, &leng, &pid, mmy, sizeof(mmy)/sizeof(mmy[0]), nd, sizeof(nd)/sizeof(nd[0]));
        if (out==1) {
            puts("Incorrect");
            return 1;
        }

        free(ad_buf);
        free(leng_buf);
        free(pid_buf);
    } else {
        close(pip1[0]);
        close(pip2[0]);
        close(pip3[0]);
        // Calculate address, length of string and pid of process, and read input
        inp = fp(&ad, &leng, &pid);
        sprintf(ad_buf, "%lx", ad);
        sprintf(leng_buf, "%lu", leng);
        sprintf(pid_buf, "%d", pid);

        write(pip1[1], ad_buf, strlen(ad_buf)+1);
        close(pip1[1]);
        write(pip2[1], leng_buf, strlen(leng_buf)+1);
        close(pip2[1]);
        write(pip3[1], pid_buf, strlen(pid_buf)+1);
        close(pip3[1]);

        // Validate input and run process for string overwrite
        sp(inp);

        free(ad_buf);
        free(leng_buf);
        free(pid_buf);
    }
    return 0;
}

// Concatenate char array to string
void c(char *buf, ...) {
    va_list args;
    va_start (args, buf);

    char arg = va_arg(args, int);
    while(arg) {
        sprintf(buf, "%s%c", buf, arg);
        arg = va_arg(args, int);
    }
    va_end (args);
}

// Decode int array by minusing int c, for length lng
char *de(int *ary, int c, int lng) {
    char *ml;
    ml = (char*)malloc(sizeof(char*));
    for (int i=0; i<lng; i++) {
        ml[i] = (char)(ary[i]-c);
    }
    return ml;
}

// Calculate address, and length of string, pid of process, and read input
char* fp(unsigned long int* ad, unsigned long* leng, int* pid) {
    *pid = getpid();
    *ad = (long unsigned int)s;
    *leng = strlen(s)+1;

    char *inp = (char*)malloc(sizeof(char*));
    fgets(inp, 255, stdin);
    return inp;
}

// Validate input and run process for string overwrite
void sp(char* inp) {
    unsigned long int i = 0;
    inp[strlen(inp)-1] = 0;
    while(s) {
        if (i!=0) {
            chk(s, inp);
            return;
        }
        printf("\nThe flag is \"%s\"!\n", s);
        puts("\nValidating......");
        sleep(1);
        i++;
    }
    return;
}

// Prints prompt
void flag(void) {
    printf("Guess the flag?\n");
}

// String compare input with overwritten string s
void chk(char* val, char* inp) {
    if (!strcmp(val, inp)) {
        puts("Correct");
    } else {
        puts("Incorrect");
    }
    return;
}

// Read /proc/{pid}/maps to find address of [heap]
int mp(unsigned long int* ad, unsigned long* leng, int* pid, int* hp, int lng1, int* mps, int lng2) {
    FILE *ptr;
    char *fl, *line;
    size_t len = 0;
    ssize_t read;
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, de(mps, 0x1A, lng2), *pid);
    ptr = fopen(fl, "r");
    if (!ptr) return 1;
    while((read = getline(&line, &len, ptr))!= -1) {
        if (strstr(line, de(hp, 0x1A, lng1))) {
            adr = strtok(line, " ");
            pm = strtok(NULL, " ");
            if (pm[0] != 'r' || pm[1] != 'w') return 1;
            
            ad1 = strtok(adr, "-");
            ad2 = strtok(NULL, "-");
            if (!ad1 || !ad2) return 1;
            break;
        }
    }
    fclose(ptr);
    free(fl);
    return 0;
}

// Read /proc/{pid}/mem to overwrite string s within process's virtual memory
int mm(unsigned long int* ad, unsigned long* leng, int* pid, int* mmy, int lng1, int* nd, int lng2) {
    char *fl, *buf = (char*)malloc(*leng);
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, de(mmy, 0x1A, lng1), *pid);
    int fd = open(fl, O_RDWR);
    if (fd == -1) return 1;

    lseek(fd, *ad, SEEK_SET);
    read(fd, buf, *leng);

    strncpy(buf, de(nd, 0x56, lng2), *leng);
    lseek(fd, *ad, SEEK_SET);
    if (write(fd, buf, *leng) == -1) {
        puts("Incorrect");
        return 1;
    }

    free(fl);
    free(buf);
    return 0;
}
