#include "../include/shell.h"
#include "../include/debug.h"

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int pos = 0;

static int ft_strcmp(const char *a, const char *b)
{
    int i = 0;

    while (a[i] && b[i])
    {
        if (a[i] != b[i])
            return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

static void halt(void)
{
    asm volatile("cli");
    while (1)
        asm volatile("hlt");
}

static inline void outb(unsigned short port, unsigned char value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void reboot(void)
{
    unsigned char status = 1;

    while (status & 0x02)
        asm volatile("inb $0x64, %0" : "=a"(status));

    asm volatile("outb %0, %1" : : "a"(0xFE), "Nd"(0x64));

    while (1)
        asm volatile("hlt");
}

void shell_init(void)
{
    printk("KFS shell\n");
    printk("> ");
    draw_cursor();   
}

static void	print_hex(uint32_t n)
{
	char	*base;
	int		i;

	base = "0123456789ABCDEF";
	printk("0x");
	i = 28;
	while (i >= 0)
	{
		terminal_putchar(base[(n >> i) & 0xF]);
		i -= 4;
	}
}

void	print_kernel_stack(void)
{
	uint32_t	*esp;
	int		i;

	asm volatile("mov %%esp, %0" : "=r"(esp));
	printk("Kernel stack:\n");
	i = 0;
	while (i < 10)
	{
		print_hex(esp[i]);
		printk("\n");
		i++;
	}
}

static void execute_command(char *cmd)
{
    if (ft_strcmp(cmd, "help") == 0)
    {
       printk("Commands:\n");
                printk("help     : show commands\n");
                printk("stack    : print kernel stack\n");
                printk("clear    : clear screen\n");
                printk("reboot   : reboot machine\n\n");
                //printk("shutdown : power off\n\n");
                //printk("halt     : halt CPU\n\n");;
                
    }
    else if (ft_strcmp(cmd, "stack") == 0)
    {
        print_kernel_stack();
    }
    else if (ft_strcmp(cmd, "clear") == 0)
    {
        terminal_initialize();
    }
    /*
    else if (ft_strcmp(cmd, "halt") == 0)
    {
        halt();
    }
    else if (ft_strcmp(cmd, "shutdown") == 0)
    {
        halt();
    }
    */
    else if (ft_strcmp(cmd, "reboot") == 0)
    {
        reboot();
    }
    else
    {
        printk("Unknown command\n");
    }
}

void shell_handle_char(char c)
{
    erase_cursor();
    if (c == '\n')
    {
        buffer[pos] = '\0';
        terminal_putchar('\n');

        execute_command(buffer);
        pos = 0;
        printk("Shell> ");
    }
    else if (c == '\b')
    {
	    if (pos > 0)
	    {
	    	pos--;
	    	buffer[pos] = '\0';
	    	terminal_putchar('\b');
	    }
    }
    else
    {
        if (pos < BUFFER_SIZE - 1)
        {
            buffer[pos++] = c;
            terminal_putchar(c);
        }
    }
    draw_cursor();
}