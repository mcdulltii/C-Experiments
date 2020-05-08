#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

void foo(void);
int change_page_permissions_of_address(void *addr);

int main(void) {
    void *foo_addr = (void*)foo;

    // Change the permissions of the page that contains foo() to read, write, and execute
    // This assumes that foo() is fully contained by a single page
    if(change_page_permissions_of_address(foo_addr) == -1) {
        fprintf(stderr, "Error while changing page permissions of foo(): %s\n", strerror(errno));
        return 1;
    }

    puts("Calling foo");
    foo();

    // http://www.exploit-db.com/exploits/13691/
    char shellcode[] =
        "\x48\x31\xd2"                              // xor    %rdx, %rdx
        "\x48\x31\xc0"                              // xor    %rax, %rax
        "\x48\xbb\x2f\x62\x69\x6e\x2f\x73\x68\x00"  // mov    $0x68732f6e69622f2f, %rbx
        "\x53"                                      // push   %rbx
        "\x48\x89\xe7"                              // mov    %rsp, %rdi
        "\x50"                                      // push   %rax
        "\x57"                                      // push   %rdi
        "\x48\x89\xe6"                              // mov    %rsp, %rsi
        "\xb0\x3b"                                  // mov    $0x3b, %al
        "\x0f\x05";                                 // syscall

    // Careful with the length of the shellcode here depending on what is after foo
    memcpy(foo_addr, shellcode, sizeof(shellcode)-1);

    puts("Calling foo");
    foo();

    return 0;
}

void foo(void) {
    int i=0;
    i++;
    printf("i: %d\n", i);
}

int change_page_permissions_of_address(void *addr) {
    // Move the pointer to the page boundary
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}
