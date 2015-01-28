ARMGNU = arm-none-eabi
CFLAGS  = -O0 -g -Wall -nostdlib -nostartfiles -ffreestanding 

NAME = volatile

all:
	$(ARMGNU)-gcc $(CFLAGS) start.s $(NAME).c -o $(NAME)
	$(ARMGNU)-objdump $(NAME) -d > $(NAME).list

clean:
	rm $(NAME) *.list 



