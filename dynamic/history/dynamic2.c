#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define hl(a)   (a) + 0x47
#define ul(a)   (a) - 0x47

typedef int (*TW)(const char*,long*,long*,char*);

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

int main() {
    long ar[] = {68,hl('y'),110,52,hl('m'),49,hl('c'),hl('_'),70,hl('u'),110,99,hl('7'),49,hl('0'),110,hl('_'),hl('C'),52,hl('1'),49,53,hl('_'),51,56,hl('4'),hl('7'),53,53,hl('6'),51,hl('7'),57,52,hl('2')};
    long rs[8];
    char sr[255];
    fgets(sr, 255, stdin);

    if (strlen(sr)==36) {
        int rC = DC("fp","iciicicciciicicicciciiciicciiciciic",(long*)ar,(long*)rs,(char*)sr);
        if (rC == 0) {
            puts("Correct");
            return 0;
        } else {
            return -1;
        }
    }

    puts("Incorrect");
}
