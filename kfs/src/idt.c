#include "../include/idt.h"
#include "../include/debug.h"
#include "../include/pic.h"
#include "../include/keyboard.h"

t_idt_segment* idt_table = (t_idt_segment*)IDT_ADDR;
extern void *idt_function[];
void	(*func_ptr[MAX_EXCEPT + 1])(void);


void general_handler(uint8_t vector)
{
	if (func_ptr[vector])
		func_ptr[vector]();
	else 
	{
		asm volatile ("cli");
		asm volatile ("hlt");
	}
}

static void create_segment(uint8_t flags, uint32_t address, uint16_t i)
{
	t_idt_segment* tmp = &idt_table[i];

	tmp->addr_low = address & 0xFFFF;
	tmp->addr_high = address >> 16 & 0xFFFF;
	tmp->reserved = 0;
	tmp->flags = flags;
	tmp->segement_offset = CS;
}

void idt_init()
{
	t_idt_ptr my_ptr;

	my_ptr.size = (uint16_t)(sizeof(t_idt_segment) * MAX_EXCEPT - 1);
	my_ptr.addr = IDT_ADDR;
	for (uint16_t i = 0; i < MAX_EXCEPT; i++)
		create_segment(INTERUPT_GATE, (uint32_t)idt_function[i] ,i);
	init_pic();
	for (uint16_t i = 0; i < 256; i++)
		func_ptr[i] = 0;
	func_ptr[PIC_OFFSET + KEYBOARD_INTERUPT] = &keyboard_handler;
	set_mask(TIME_INTERUPT);
	clear_mask(KEYBOARD_INTERUPT);
	asm volatile("lidt %0" : :"m"(my_ptr));
	asm volatile("sti");
}
