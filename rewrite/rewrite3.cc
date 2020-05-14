#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void sp(int);
void fp(unsigned long int*, unsigned long*, int*);
int mp(unsigned long int*, unsigned long*, int*, int*, int, int*, int);
int mm(unsigned long int*, unsigned long*, int*, int*, int, int*, int);
char *de(int*, int, int);
void c(char *buf, ...);

char s[512]={0};
char *adr, *perm, *ad1, *ad2;
int out, cnt = 2;

int main(void)
{
    pid_t f;
    char *ad_buf=(char*)malloc(sizeof(char*)), *leng_buf=(char*)malloc(sizeof(char*)), *pid_buf=(char*)malloc(sizeof(char*));
    unsigned long int ad;
    unsigned long leng;
    int pid;

    int hp[] = {0x75,0x82,0x7f,0x7b,0x8a,0x77,0x1a,};
    int mps[] = {0x49,0x8a,0x8c,0x89,0x7d,0x49,0x3f,0x7e,0x49,0x87,0x7b,0x8a,0x8d,0x1a,};
    int mmy[] = {0x49,0x8a,0x8c,0x89,0x7d,0x49,0x3f,0x7e,0x49,0x87,0x7f,0x87,0x1a,};
    int nd[] = {0x8b,0x89,0x87,0xbc,0xb5,0xc3,0x86,0xba,0x87,0xbc,0xcf,0x87,0xc4,0xbd,0xb5,0x8a,0x87,0x8d,0x89,0xc8,0xc4,0x8a,0x8d,0x87,0xcc,0x89,0xb5,0x89,0x8f,0x8a,0x8d,0x8b,0x8e,0x88,0x8f,0x56,};

    int pip1[2];
    int pip2[2];
    int pip3[2];
    if (pipe(pip1)==-1 || pipe(pip2)==-1 || pipe(pip3)==-1) {
        puts("Incorrect");
        return 1;
    }

    c(s, 'T','h','i','s',' ','I','n','i','t','i','a','l',' ','D','a','t','a',' ','i','s',' ','s','t','i','l','l',' ','n','o','t',' ','e','n','o','u','g','h','\0');
    if (s == NULL)
    {
        puts("Incorrect");
        return 1;
    }
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
        
        ad = strtoul(ad_buf, NULL, 16);
        leng = strtol(leng_buf, NULL, 10);
        pid = strtol(pid_buf, NULL, 10);

        out = mp(&ad, &leng, &pid, hp, sizeof(hp)/sizeof(hp[0]), mps, sizeof(mps)/sizeof(mps[0]));
        if (out==1) {
            puts("Incorrect");
            return 1;
        }
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
        fp(&ad, &leng, &pid);
        sprintf(ad_buf, "%lx", ad);
        sprintf(leng_buf, "%lu", leng);
        sprintf(pid_buf, "%d", pid);

        write(pip1[1], ad_buf, strlen(ad_buf)+1);
        close(pip1[1]);
        write(pip2[1], leng_buf, strlen(leng_buf)+1);
        close(pip2[1]);
        write(pip3[1], pid_buf, strlen(pid_buf)+1);
        close(pip3[1]);

        sp(cnt);

        free(ad_buf);
        free(leng_buf);
        free(pid_buf);
    }
    return 0;
}

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

char *de(int *ary, int c, int lng)
{
    char *ml;
    ml = (char*)malloc(sizeof(char*));
    for (int i=0; i<lng; i++) {
        ml[i] = (char)(ary[i]-c);
    }
    return ml;
}

void fp(unsigned long int* ad, unsigned long* leng, int* pid) {
    *pid = getpid();
    *ad = (long unsigned int)s;
    *leng = strlen(s)+1;
}

void sp(int num) {
    unsigned long int i;
    i = 0;
    while (s)
    {
        if (i==num) return;
        printf("[%lu] %s\n", i, s);
        sleep(1);
        i++;
    }
    return;
}

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
            perm = strtok(NULL, " ");
            if (perm[0] != 'r' || perm[1] != 'w') return 1;
            
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
