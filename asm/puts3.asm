  BITS 32
                org     0x00200000
  PROT_READ     equ     0x1                     ; page can be read
  PROT_WRITE    equ     0x2                     ; page can be written

  ehdr:
                db      0x7F, "ELF"             ; e_ident
                db      1, 1, 1
  _start:       mov     dl, len                 ; length
                mov     ecx, msg                ; msg buffer
                jmp     _main
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

  _main:        call    _print
                mov     al, 125                 ; mprotect
                mov     ebx, 0x200000           ; multiple of pagesize
                mov     ecx, 4096               ; pagesize
                mov     dl, PROT_READ
                or      edx, PROT_WRITE
                int     0x80

                mov     dl, 20                  ; length
                mov     ecx, input              ; input buffer
                mov     ebx, 2
                mov     al, 3
                int     0x80

                test    eax, eax                ; check if stdin is read
                jle     EOF

                mov     esi, key                ; key
                mov     edi, input              ; input
                mov     ecx, klen+1             ; len(key)
                cld
                repe    cmpsb                   ; strncmp(key, input, len(key))
                jecxz   decode                  ; if ecx == 0

                mov     edx, wlen               ; length
                mov     ecx, wrong              ; incorrect buffer
                call    _print

  EOF:          xor     eax, eax                ; _exit()
                inc     eax
                int     0x80

  decode:       mov     edx, flen
                mov     ecx, flag
                call    _print
                jmp     EOF
  
  _print:       mov     bl, 1
                mov     al, 4
                int     0x80
                ret

  section .data
                msg     db "Guess the flag?", 0xa
  len           equ     $ - msg
                wrong   db "Incorrect", 0xa
  wlen          equ     $ - wrong

  section .bss
                input   resb 200
  inlen         equ     $ - input

  section .rodata
                key     db "hello", 0xa
  klen          equ     $ - key
                flag    db "A5M_15n't_th4t_34sy_4ft3r_4ll_4857614902", 0xa
  flen          equ     $ - flag

  filesize      equ     $ - $$
