#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    puts("HOII");
    char* c;
    c = (char*)malloc(sizeof(char*));
    fgets(c, 10, stdin);
    printf("HELLO %d", 0x16);
}
