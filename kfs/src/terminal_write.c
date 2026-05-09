#include "../include/debug.h"

uint8_t		terminal_row;
uint8_t		terminal_column;
uint8_t		terminal_color;
uint16_t	*terminal_buffer = (uint16_t *)VGA_MEMORY;

enum vga_color {
	VGA_COLOR_BLACK        = 0,
	VGA_COLOR_BLUE         = 1,
	VGA_COLOR_GREEN        = 2,
	VGA_COLOR_CYAN         = 3,
	VGA_COLOR_RED          = 4,
	VGA_COLOR_MAGENTA      = 5,
	VGA_COLOR_BROWN        = 6,
	VGA_COLOR_LIGHT_GREY   = 7,
	VGA_COLOR_DARK_GREY    = 8,
	VGA_COLOR_LIGHT_BLUE   = 9,
	VGA_COLOR_LIGHT_GREEN  = 10,
	VGA_COLOR_LIGHT_CYAN   = 11,
	VGA_COLOR_LIGHT_RED    = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN  = 14,
	VGA_COLOR_WHITE        = 15,
};

static inline void outb(unsigned short port, unsigned char value)
{
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void disable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static inline uint8_t	vga_color(uint8_t fg, uint8_t bg)
{
	return (fg | bg << 4);
}

static inline uint16_t	vga_entry(unsigned char c, uint8_t color)
{
	return ((uint16_t)c | (uint16_t)color << 8);
}

void scroll(void)
{
    for (int y = 1; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] =
                terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++)
    {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            vga_entry(' ', terminal_color);
    }
}

void draw_cursor(void)
{
	terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] =
		vga_entry('|', terminal_color);
}

void erase_cursor(void)
{
	terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] =
		vga_entry(' ', terminal_color);
}

void terminal_putchar(char c)
{
	if (c == '\b')
	{
		if (terminal_column > 0)
		{
			terminal_column--;
			terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] =
				vga_entry(' ', terminal_color);
		}
		return;
	}

	if (c == '\n')
	{
		terminal_column = 0;
		terminal_row++;
	}
	else
	{
		terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] =
			vga_entry(c, terminal_color);
		terminal_column++;
	}

	if (terminal_column >= VGA_WIDTH)
	{
		terminal_column = 0;
		terminal_row++;
	}

	if (terminal_row >= VGA_HEIGHT)
	{
		scroll();
		terminal_row = VGA_HEIGHT - 1;
	}
}

void	terminal_initialize(void)
{
	int	i;
	int	j;

	terminal_row    = 0;
	terminal_column = 0;
	terminal_color  = vga_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
	for (i = 0; i < VGA_HEIGHT; i++)
		for (j = 0; j < VGA_WIDTH; j++)
			terminal_buffer[i * VGA_WIDTH + j] = vga_entry(' ', terminal_color);
}

void	terminal_write_string(char *str)
{
	while (*str)
        terminal_putchar((unsigned char)*str++);
}
