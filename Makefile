ASM_FILES= boot.s set_gdt.s set_idt.s
C_FILES= kernel.c gdt.c print.c terminal_write.c idt.c pic.c keyboard.c

PREFIX = kfs/src/
SRC_C = $(addprefix $(PREFIX), $(C_FILES))
SRC_ASM = $(addprefix $(PREFIX), $(ASM_FILES))
NAME = kfs.iso

all: $(NAME)

$(NAME): $(SRC_ASM) $(SRC_C)
	docker compose up --build
	cp kfs/$(NAME) .

run : $(NAME)
	qemu-system-i386 -cdrom $(NAME)

clean:
	cd kfs && make fclean
	rm -rf $(NAME)

re : clean
	cd 
	make all
