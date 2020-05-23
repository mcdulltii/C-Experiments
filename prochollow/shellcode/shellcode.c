#include <stdio.h>

/*
int foo() {
  return 10;
}

> gcc -c -O3 shellcode.c
> objdump -d shellcode.o

-> \xb8\x0a\x00\x00\x00\xc3
 */
unsigned char code[] = "\xb8\x0a\x00\x00\x00\xc3";

int main() {
    int foo_value = 0;

    int (*foo)() = (int(*)())code;
    foo_value = foo();

    printf("%d\n", foo_value);
}

// > gcc -fno-stack-protector -z execstack -o shellcode shellcode.c
