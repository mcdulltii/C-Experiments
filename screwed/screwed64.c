#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/procfs.h>
#include <fcntl.h>

//typedef struct elf64_hdr {
//    unsigned char e_ident[16];      /* ELF "magic number" */
//    Elf64_Half e_type;
//    Elf64_Half e_machine;
//    Elf64_Word e_version;
//    Elf64_Addr e_entry;     /* Entry point virtual address */
//    Elf64_Off e_phoff;      /* Program header table file offset */
//    Elf64_Off e_shoff;      /* Section header table file offset */
//    Elf64_Word e_flags;
//    Elf64_Half e_ehsize;
//    Elf64_Half e_phentsize;
//    Elf64_Half e_phnum;
//    Elf64_Half e_shentsize;
//    Elf64_Half e_shnum;
//    Elf64_Half e_shstrndx;
//    
//} Elf64_Ehdr;

int main(int argc, char** argv){
    printf(".: Elf corrupt :.\n");

    if(argc < 2){
        printf("Usage: %s file\n", argv[0]);
        return 1;
    }
    else{

        int f;
        static Elf64_Ehdr* header;

        if((f = open(argv[1], O_RDWR)) < 0){
            perror("open");
            return 1;
        }

        if((header = (Elf64_Ehdr *) mmap(NULL, sizeof(header), PROT_READ |
                        PROT_WRITE, MAP_SHARED, f, 0)) == MAP_FAILED){
            perror("mmap");
            close(f);
            return 1;
        }
        printf("[*] Current e_shnum: %d\n", header->e_shnum);
        
        header->e_shoff = 0xffff;
        header->e_shnum = 0xffff;
        header->e_shstrndx = 0xffff;

        printf("[*] Patched e_shnum: %d\n", header->e_shnum);

        if(msync(NULL, 0, MS_SYNC) == -1){
            perror("msync");
            close(f);
            return 1;
        }

        close(f);
        munmap(header, 0);
    }
    return 0;
}
