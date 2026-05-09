%define FLAGS 0x3
%define MAGIC 0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text
global _start
extern kernel_main
_start:
	mov esp, stack_top
	pushfd
	pushad
	call kernel_main

	cli
.hang:	
	hlt
	jmp .hang

