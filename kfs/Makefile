SRC_ASM=src/boot.s src/set_gdt.s src/set_idt.s 

OBJ_ASM=$(SRC_ASM:src/%.s=obj/%.o)

SRC_C=src/idt.c src/kernel.c src/gdt.c src/print.c src/terminal_write.c src/pic.c src/keyboard.c src/shell.c

OBJ_C=$(SRC_C:src/%.c=obj/%.o)

LINKER=linker.ld

AS=nasm

AS_FLAGS= -f elf32

GCC= i686-elf-gcc

GCC_FLAGS= -ffreestanding -nostdlib -fno-pic -fno-asynchronous-unwind-tables -fno-unwind-tables

LINK_FLAGS= -ffreestanding -nostdlib -no-pie -Wl,--build-id=none

NAME=kfs

all: $(NAME) image

image: $(NAME) $(OBJ_C) $(OBJ_ASM)
	mkdir -p isodir/boot/grub
	cp $(NAME) isodir/boot/$(NAME)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME).iso isodir

$(NAME): $(OBJ_ASM) $(OBJ_C)
	$(GCC) -T $(LINKER)  -o $(NAME) $(LINK_FLAGS) $(OBJ_ASM) $(OBJ_C)

obj/%.o: src/%.s
	mkdir -p obj
	$(AS) $(AS_FLAGS) -o $@ $<

obj/%.o: src/%.c
	mkdir -p obj
	$(GCC) $(GCC_FLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ_ASM) $(OBJ_C)

fclean: clean
	rm -rf $(NAME) $(NAME).iso isodir

re: fclean
	make all
