#include <stdint.h>
#include <stdarg.h>
#include "../include/gdt.h"
#include "../include/debug.h"
#include "../include/idt.h"

#include "../include/debug.h"
#include "../include/shell.h"

#include "../include/debug.h"
#include "../include/shell.h"

void kernel_main(void)
{
	terminal_initialize();
	disable_cursor();   

	init_gdt();
	idt_init();

	asm volatile("sti");
    shell_init();

	while (1)
		asm volatile("hlt");
}