#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef unsigned char BYTE;

char* maps(char*, int);
int mem(char*, char*, char*);

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    int count = 1;
    char* pid = argv[1];
    char *addr[10], *start_addr, *end_addr;

    while(!strstr(maps(pid, count), "[heap]")) {
        addr[count] = maps(pid, count);
        if (count == 1) start_addr = strtok(addr[count], "-");
        count++;
    }
    end_addr = strtok(maps(pid, count-1), " ");
    end_addr = strtok(end_addr, "-");
    end_addr = strtok(NULL, "-");

    mem(pid, start_addr, end_addr);
    return 0;
}

char* maps(char* pid, int linesearch) {
    FILE *ptr;
    int line = 1, found = 0;
    char* fl, liner[100], *rtn = (char*)malloc(sizeof(char*));
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%s/maps", pid);
    ptr = fopen(fl,"r");
    if (ptr == NULL) exit(1);
    
    while(fgets(liner, 100, ptr)) {
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
    int i;
    BYTE *buf = (BYTE*)malloc(length);
    fl = (char*)malloc(sizeof(char*));
    sprintf(fl, "/proc/%s/mem", pid);
    int fd = open(fl, O_RDWR);
    if (fd == -1) return 1;
    
    lseek(fd, s_addr, SEEK_SET);
    read(fd, buf, length);
    for (i=0; i<length; i++) {
        //printf("%d\n", buf[i]);
        if ((int)buf[i] != 0x55) continue;
        if ((int)buf[i+1] != 0x48) continue;
        if ((int)buf[i+2] != 0x89) continue;
        if ((int)buf[i+3] != 0xe5) continue;
        break;
    }
    lseek(fd, s_addr+i, SEEK_SET);
    read(fd, buf, length);
    for (int j=0; j<10; j++) {
        printf("%x\n", buf[j]);
    }

    free(fl);
    free(buf);
    return 0;
}
