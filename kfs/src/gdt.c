#include "../include/gdt.h"


void initialize_segment(s_gdt_segment* seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	seg->base_low = base & 0xFFFF;
	seg->base_mid = base >> 16 & 0xFF;
	seg->base_high = base >> 24 & 0xFF;
	seg->access = access;
	seg->flags = flags << 4 | limit >> 16 & 0xF;
	seg->limit_low = limit & 0xFFFF;
}

void	init_gdt()
{
	s_gdt_ptr	ptr;

	ptr.size = 8 * 7 - 1;
	ptr.addr = GDT_ADDR; 
	initialize_segment((s_gdt_segment*)ptr.addr, 0, 0, 0, 0);
	initialize_segment((s_gdt_segment*)ptr.addr + 1, 0x00000000, 0xFFFFF, KERNEL_CODE, FLAGS);
	initialize_segment((s_gdt_segment*)ptr.addr + 2, 0x00000000, 0xFFFFF, KERNEL_DATA, FLAGS);
	initialize_segment((s_gdt_segment*)ptr.addr + 3, 0x00000000, 0xFFFFF, KERNEL_STACK, FLAGS);
	initialize_segment((s_gdt_segment*)ptr.addr + 4, 0x00000000, 0xFFFFF, USER_CODE, FLAGS);
	initialize_segment((s_gdt_segment*)ptr.addr + 5, 0x00000000, 0xFFFFF, USER_DATA, FLAGS);
	initialize_segment((s_gdt_segment*)ptr.addr + 6, 0x00000000, 0xFFFFF, USER_STACK, FLAGS);
	// need to initialize tss for interupt and ring level switch
	asm volatile ("lgdt %0": : "m"(ptr));
	set_gdt();
}
