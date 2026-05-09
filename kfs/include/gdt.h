#ifndef GDT_H
#define GDT_H
#include "stdint.h"

#define GDT_ADDR 0x0800
#define FLAGS 0x0C //0b1100 G = 1 means its limit times 4Kib and not 1 bytes D/B = 1 means 32 bit protected mode L = 0 no 64 bits
#define KERNEL_CODE 0x9A //0x9 for kernel 0b1001 P = 1 valid segment DPL = 00 kernel privileges E = 1 code/data segment 
#define KERNEL_STACK 0x96
#define KERNEL_DATA 0x92
#define USER_CODE 0xFA	// 0xA 0b1010 for description of read / write / exec E=1 executable DC=0 not shared with other privileges RW=1 read acces for exec A=0 accessed bit manage by cpu
#define USER_STACK 0xF6 // 0x6 0b0110 E= 0 data segment DC = 1 grow down RW=1 write and read 
#define USER_DATA 0xF2 // 0x2 0b0010 E = 0 data segment DC =0 grow up not down RW=1 write and read A=0 manage by cpu 

extern uint8_t		terminal_row;
typedef struct {
	uint16_t	limit_low;
	uint16_t	base_low;
	uint8_t		base_mid;
	uint8_t		access;
	uint8_t		flags;
	uint8_t		base_high;
}	__attribute__((packed)) s_gdt_segment;


typedef struct {
	uint16_t	size;
	uint32_t	addr;
}	__attribute__((packed)) s_gdt_ptr;

void initialize_segment(s_gdt_segment* seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void	init_gdt();
void	set_gdt();
#endif
