#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int (*TWrapper)(const char*,long*,long*);

int pf(const char* types,long* args,long* results)
{
    // Function specific code, in this case you can call printf for each parameter
    while(*types)
    {
        switch(*types){
        case 'i':
            printf("%d",(int)*args);
            break;
        case 'c':
            printf("%c",(char)*args);
            break;
        }
        ++types;
        ++args;
    }
    // Return number of filled results
    return 0;
}

int WrapFoo(const char* types,long* args,long* results)
{
    // ..function specific code..
    return 0;
}

typedef struct STableItem{
    const char *strName;
    TWrapper pFunc;
} STableItem;

STableItem table[] = {
    {"printf", &pf},
    {"foo", &WrapFoo},
    {NULL, NULL}
};

int DynamicCall(const char *func_name,const char* types,long* args,long* results)
{
    int k;
    for(k=0;table[k].strName != NULL;++k){
        if(strcmp(func_name,table[k].strName) == 0){
            return table[k].pFunc(types,args,results);
        }
    }
    return -1;
}

int main() {
    long args[] = {123,'b'};
    long results[8];
    
    // Dynamic function call
    int res_count = DynamicCall("printf","ic",(long*)args,(long*)results);

    // Achieved output
    puts("\n123b");
}
