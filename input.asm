section .data
	a dd 10
	b dd 12
	c dw 1
section .bss
	temp resb 11
	temp2 resd 5
	t resd 4
section .text
	global main

main:
	mov eax,4
	mov ebx,ecx
	mov ecx,a
	mov edx,13
	add eax,dword[a]

	mov eax,1
	xor ebx,ebx
	jmp ab
ab:
	add eax,ebx
