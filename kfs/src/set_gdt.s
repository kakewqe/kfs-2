section .text
global set_gdt

set_gdt:
	jmp 0x8:set_reg

set_reg:
	mov ax, 0x18 
	mov ss, ax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	ret


