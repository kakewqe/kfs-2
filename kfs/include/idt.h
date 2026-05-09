#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#define MAX_EXCEPT 256
#define CS 0x08
#define INTERUPT_GATE 0x8E
#define TRAP_GATE 0x8F
#define IDT_ADDR 0xF00
#define TIME_INTERUPT 0x00
#define KEYBOARD_INTERUPT 0x01

typedef struct {
	uint16_t	addr_low;
	uint16_t	segement_offset;
	uint8_t		reserved;
	uint8_t		flags;
	uint16_t	addr_high;
}	__attribute__((packed))t_idt_segment;

typedef struct {
	uint16_t	size;
	uint32_t	addr;
}	__attribute__((packed))t_idt_ptr;

void general_handler(uint8_t vector);
void idt_init();
#endif
