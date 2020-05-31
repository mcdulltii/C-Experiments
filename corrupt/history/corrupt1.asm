  BITS 32
                org     0x00200000

  ehdr:
                db      0x7F, "ELF"             ; e_ident
                db      1, 1, 1
  _start:       jmp     _main
        times 7 db      0
                dw      2                       ; e_type
                dw      3                       ; e_machine
                dd      1                       ; e_version
                dd      _start                  ; e_entry
                dd      phdr - $$               ; e_phoff
  phdr:         dd      1                       ; e_shoff       ; p_type
                dd      0                       ; e_flags       ; p_offset
                dd      $$                      ; e_ehsize      ; p_vaddr
                                                ; e_phentsize
                dw      1                       ; e_phnum       ; p_paddr
                dw      0                       ; e_shentsize
                dd      filesize                ; e_shnum       ; p_filesz
                                                ; e_shstrndx
                dd      filesize                                ; p_memsz
                dd      5                                       ; p_flags
                dw      0x1                                     ; p_align

  _main:        mov     edx, len
                mov     ecx, msg
                mov     ebx, 1
                mov     eax, 4
                int     0x80

                xor     eax, eax                ; _exit()
                inc     eax
                int     0x80
  
  section .data
                msg     db "Guess the flag?", 0xa
  len           equ     $ - msg

  filesize      equ     $ - $$
