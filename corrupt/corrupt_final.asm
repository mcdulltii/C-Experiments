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

                mov     dl, 50                  ; length
                mov     ecx, input              ; input buffer
                mov     ebx, 2
                mov     al, 3                   ; sys_read
                int     0x80

                test    eax, eax                ; check if stdin is read
                jle     EOF
                
                mov     esi, key                ; key
                mov     edi, input              ; input
                mov     ecx, klen-19            ; len(key)
                cld
                repe    cmpsb                   ; strncmp(key, input, len(key))
                jecxz   decode                  ; if ecx == 0

                mov     edx, wlen               ; length
                mov     ecx, wrong              ; incorrect buffer
                call    _print

  EOF:          xor     eax, eax                ; _exit()
                inc     eax
                int     0x80

  decode:       xor     ebx, ebx                ; index
  cycle:        mov     al, byte [buf + ebx]    ; flag buffer index
                ;xor     al, 0x39                ; decode with xor 0x39
                mov     cl, byte [input + klen - 1]
                xor     al, cl
                ;sub     al, 0x64                ; decode with sub 0x64
                mov     dl, byte [input + klen - 2]
                sub     al, dl
                mov     [flag + ebx], eax       ; copy to flag string buffer
                inc     ebx
                cmp     ebx, blen               ; end of flag buffer
                jne     cycle

                mov     edx, clen
                mov     ecx, correct
                call    _print
                mov     edx, flen               ; length
                mov     ecx, flag               ; flag string buffer
                call    _print
                jmp     EOF
  
  _print:       mov     bl, 1
                mov     al, 4
                int     0x80
                ret

  section .rodata
                msg     db "Guess the flag?", 0xa
  len           equ     $ - msg
                wrong   db "Incorrect", 0xa
  wlen          equ     $ - wrong
                correct db "Flag: "
  clen          equ     $ - correct
                key    db "Why_c4nt_1_d3c0mp1l3_th1s", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    ; "Why_c4nt_1_d3c0mp1l3_th1s__________________d9"
  klen          equ     $ - key

  section .bss
                input   resb 200
  inlen         equ     $ - input
                flag    resb 100                ; "A5M_15n't_th4t_34sy_4ft3r_4ll_4857614902", 0xa
  flen          equ     $ - flag

  section .data
                buf     db 0x9c, 0xa0, 0x88, 0xfa, 0xac, 0xa0, 0xeb, 0xb2, 0xe1, 0xfa, 0xe1, 0xf5, 0xa1, 0xe1, 0xfa, 0xae, 0xa1, 0xee, 0xe4, 0xfa, 0xa1, 0xf3, 0xe1, 0xae, 0xef, 0xfa, 0xa1, 0xe9, 0xe9, 0xfa, 0xa1, 0xa5, 0xa0, 0xa2, 0xa3, 0xac, 0xa1, 0xa4, 0xad, 0xaf, 0x57
  blen          equ     $ - buf

  filesize      equ     $ - $$
