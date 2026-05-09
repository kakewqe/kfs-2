#ifndef DEBUG_H
# define DEBUG_H
#include <stdarg.h>
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

typedef struct {
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax, eflags;
}	t_regs;

void	printk(char *str, ...);
void	terminal_putchar(char c);
void	terminal_write_string(char *str);
void	terminal_initialize(void);

void	clear_screen(void);
void	print_kernel_stack(void);
void	draw_cursor(void);
void	erase_cursor(void);
void	disable_cursor(void);

#endif
