#include "../include/pic.h"
#include "../include/debug.h"

uint8_t	inb(uint16_t port)
{
	uint8_t	ret;

	asm volatile("inb %w1, %b0":"=a"(ret): "Nd"(port):"memory");
	return (ret);
}

void	outb(uint16_t port, uint8_t val)
{
	asm volatile ("outb %b0, %w1"::"a"(val), "Nd"(port): "memory");
}
void io_wait(void)
{
    outb(0x80, 0);
}

static void PIC_remap(uint8_t offset1, uint8_t offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	outb(PIC1_DATA, 1 << CASCADE_IRQ);
	io_wait();
	outb(PIC2_DATA, CASCADE_IRQ);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

void set_mask(uint8_t index)
{
	if (index < 8 && index != CASCADE_IRQ)
		outb(PIC1_DATA, 1 << index | inb(PIC1_DATA));
	else
		outb(PIC2_DATA, 1 << (index - 8) | inb(PIC2_DATA));
}

void clear_mask(uint8_t index)
{
	if (index < 8 && index != CASCADE_IRQ)
		outb(PIC1_DATA, ~(1 << index) & inb(PIC1_DATA));
	else
		outb(PIC2_DATA, ~(1 << (index - 8)) & inb(PIC2_DATA));
}

void	PIC_sendEOI(uint8_t irq)
{
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void	init_pic()
{
	PIC_remap(PIC_OFFSET, PIC_OFFSET + 8);
}


