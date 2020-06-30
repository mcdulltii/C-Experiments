  ; reboot.asm
  BITS 32
  GLOBAL main
  SECTION .text
  main:
			xor eax, eax            ; Clearing the EAX register
			xor ebx, ebx            ; Clearing the EBX register
			xor ecx, ecx            ; Clearing the ECX register
			cdq                     ; Clearing the EDX register
			mov al, 0x58            ; Loading syscall value = 0x58 for reboot in AL
			mov ebx, 0xfee1dead     ; Loading magic 1 in EBX
			mov ecx, 672274793      ; Loading magic 2 in ECX
			mov edx, 0x1234567      ; Loading cmd val = LINUX_REBOOT_CMD_RESTART in EDX
			int 0x80                ; Executing the reboot syscall

