#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void sp(int);
void fp(unsigned long int*, unsigned long*, int*);
int mp(unsigned long int*, unsigned long*, int*);
int mm(unsigned long int*, unsigned long*, int*);

char *s;
char *addr, *perm, *addr1, *addr2;
int out, cnt = 2;

int main(void)
{
    pid_t f;
    char *ad_buf=(char*)malloc(sizeof(char*)), *leng_buf=(char*)malloc(sizeof(char*)), *pid_buf=(char*)malloc(sizeof(char*));
    unsigned long int ad;
    unsigned long leng;
    int pid;
    int pip1[2];
    int pip2[2];
    int pip3[2];
    if (pipe(pip1)==-1 || pipe(pip2)==-1 || pipe(pip3)==-1) {
        puts("Pipe Failed");
        return 1;
    }

    s = strdup("Initial Data");
    if (s == NULL)
    {
        puts("Can't allocate mem with malloc");
        return 1;
    }
    f = fork();
    pid = 0;
    if (f < 0) {
        puts("Error forking");
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

        out = mp(&ad, &leng, &pid);
        if (out==1) {
            puts("Error");
            return 1;
        }
        out = mm(&ad, &leng, &pid);
        if (out==1) {
            puts("Error");
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

int mp(unsigned long int* ad, unsigned long* leng, int* pid) {
    FILE *ptr;
    char *fl, *line;
    size_t len = 0;
    ssize_t read;
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%d/maps", *pid);
    ptr = fopen(fl, "r");
    if (!ptr) return 1;
    while((read = getline(&line, &len, ptr))!= -1) {
        if (strstr(line, "[heap]")) {
            addr = strtok(line, " ");
            perm = strtok(NULL, " ");
            if (perm[0] != 'r' || perm[1] != 'w') return 1;
            
            addr1 = strtok(addr, "-");
            addr2 = strtok(NULL, "-");
            if (!addr1 || !addr2) return 1;
            break;
        }
    }
    fclose(ptr);
    free(fl);
    return 0;
}

int mm(unsigned long int* ad, unsigned long* leng, int* pid) {
    char *fl, *buf = malloc(*leng);
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%d/mem", *pid);
    int fd = open(fl, O_RDWR);
    if (fd == -1) return 1;

    lseek(fd, *ad, SEEK_SET);
    read(fd, buf, *leng);

    strncpy(buf, "New Data", *leng);
    lseek(fd, *ad, SEEK_SET);
    if (write(fd, buf, *leng) == -1) {
        puts("Error overwriting");
        return 1;
    }

    free(fl);
    free(buf);
    return 0;
}
