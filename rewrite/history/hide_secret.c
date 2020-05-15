#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VNAMEMAX 20
#define VVALUEMAX 200

typedef struct
{
    char vname[VNAMEMAX];
    char vvalue[VVALUEMAX];
}VDEF_T;


VDEF_T v[] =
{
    {.vname = "hp",.vvalue = "[heap]"},
    {.vname = "mps",.vvalue = "/proc/%d/maps" },
    {.vname = "mmy",.vvalue = "/proc/%d/mem" },
    {.vname = "nd",.vvalue = "531f_m0d1fy1ng_4173rn471v3_39475829" },
    {0,},
};

int code(VDEF_T *v, char *fname, int c)
{
    FILE *fp = fopen(fname, "wt");
    int result = (fp == NULL) * -1;

    if (!result)
    {
        fprintf(fp, "#ifndef _SEC\n#define _SEC\n\n\n");
        while (!result && v->vname[0])
        {
            fprintf(fp, "static unsigned char %s[] = {\n\t\t", v->vname);
            for (int i = 0; i <= strlen(v->vvalue); i++)
            {
                if (fprintf(fp, "0x%02x,", v->vvalue[i] + c) < 0)
                {
                    result = -1;
                    break;
                }
            }
            if (fprintf(fp, "};\n\n") < 0) result = -1;
            v++;
        }
        if(fprintf(fp, "#endif") < 0) result = -1;
        fclose(fp);
    }
    return result;
}

int main() {
    code(v, "temp", 0x56);
}
