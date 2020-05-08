#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#define hl(a)   (a) + 0x47
#define ul(a)   (a) - 0x47

typedef int (*TW)(const char*,long*,long*,char*);
int bf (void *ad); 
void ch (void);
void __attribute__((constructor)) flag (void); 

int gdb(void *add) {
    if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1) return 1;
    unsigned char *ins = (unsigned char*)add + 84;
    *ins = 0x24;
    return 0;
}

int pf(const char* ty,long* ar,long* rs, char* in)
{
    while(*ty)
    {
        switch(*ty){
        case 'i':
            if ((int)*ar != (int)*in) {
                puts("Incorrect");
                return -1;
            }
            break;
        case 'c':
            if ((int)ul(*ar) != (int)*in) {
                hl(*ar);
                puts("Incorrect");
                return -1;
            }
            hl(*ar);
            break;
        }
        ++ty;
        ++ar;
        ++in;
    }
    return 0;
}

typedef struct ST{
    const char *SN;
    TW pF;
} ST;

ST tb[] = {
    {"fp", &pf},
    {NULL, NULL}
};

int DC(const char *fc,const char* ty,long* ar,long* rs,char* in)
{
    int k;
    for(k=0;tb[k].SN != NULL;++k){
        if(strcmp(fc,tb[k].SN) == 0){
            return tb[k].pF(ty,ar,rs,in);
        }
    }
    return -1;
}

int bf(void *ad) {
    int pS = getpagesize();
    ad -= (unsigned long)ad % pS;
    if(mprotect(ad, pS, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }
    return 0;
}

int fb(void *ad) {
    int pS = getpagesize();
    ad -= (unsigned long)ad % pS;
    if(mprotect(ad, pS, PROT_EXEC) == -1) {
        return -1;
    }
    return 0;
}

void flag(void) {
    printf("Guess the flag?\n");
}

void ch(void) {
    long ar[] = {68,hl('y'),110,52,hl('m'),49,hl('c'),hl('_'),70,hl('u'),110,99,hl('7'),49,hl('0'),110,hl('_'),hl('C'),52,hl('1'),49,53,hl('_'),51,56,hl('4'),hl('7'),53,53,hl('6'),51,hl('7'),57,52,hl('2')};
    long rs[8];
    char sr[255];
    fgets(sr, 255, stdin);

    if (strlen(sr)==20) {
        int rC = DC("fp","iciicicciciicicicciciiciicciiciciic",(long*)ar,(long*)rs,(char*)sr);
        if (rC == 0) {
            puts("Correct");
        }
        return;
    }
    puts("Incorrect");
}

int main(void) {
    void *add = (void*)ch;
    if (bf(add) == -1) {
        puts("Incorrect");
    }
    if (gdb(add)) exit(0);
    ch();
    fb(add);
    return 0;
}
