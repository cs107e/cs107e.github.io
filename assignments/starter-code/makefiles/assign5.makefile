MY_MODULES = keyboard.o shell.o 

# MY_MODULES is a list of those library modules (such as gpio.o)
# for which you intend to use your own code. The reference implementation
# from our libraries will be used for any module you do not name in this list.
# Editing this list allows you to control whether the application being
# built is using your code or the reference implementation for each module
# on a per-module basis. Great for testing!
#
# NOTE: when you name a module in this list, it must provide definitions
# for all of the symbols in the entire module. For example, if you list
# gpio.o as one of your modules, your gpio.o must define gpio_set_function,
# gpio_get_function, ... and so on for all functions declared in the gpio.h
# header file. If your module forgets to implement any of the needed
# functions, the linker will bring in gpio.o from reference libpi to
# resolve the missing definition. But you can't have both gpio.o modules!
# The linker will report multiple definition errors for every function
# that occurs in both your gpio.o and the reference gpio.o. No bueno!
#
# You shouldn't need to modify anything below this line.
########################################################

# Paths to binaries.
APPLICATION = build/bin/uart_shell.bin
TEST 		= build/bin/test_keyboard.bin

# Use order-only prerequisite to create the build directory.
# https://www.cmcrossroads.com/article/making-directories-gnu-make
# This comment is floating all alone because a previous edition of this
# Makefile had a macro for the build directory, but that has since been
# removed.

# Object files needed to build the application binary.
OBJECTS = $(addprefix build/obj/, $(MY_MODULES) start.o cstart.o)

# C compiler.
CFLAGS_EXTRA = -Werror
CFLAGS 	= -I$(CS107E)/include -Iinclude -Og -g -Wall -std=c99 -ffreestanding $(CFLAGS_EXTRA)
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -Wpointer-arith
LDFLAGS	= -nostdlib -T src/boot/memmap -L$(CS107E)/lib
LDLIBS 	= -lpi -lgcc -lref_pi

# Search for .c and .s files in the src directory's subdirectories.
# https://www.cmcrossroads.com/article/basics-vpath-and-vpath
vpath %.c src/apps src/boot src/lib src/tests
vpath %.s src/apps src/boot src/lib src/tests


all: $(APPLICATION) $(TEST) 

# Ensure that `make <file>.<ext>` builds `<file>.<ext>` in `build/<ext>/`.
%.bin: build/bin/%.bin ;
%.elf: build/elf/%.elf ;
%.o: build/obj/%.o ;
%.list: build/list/%.list ;

# Build *.bin from *.elf.
build/bin/%.bin: build/elf/%.elf | build
	arm-none-eabi-objcopy $< -O binary $@

# Build *.elf from *.o.
build/elf/%.elf: build/obj/%.o $(OBJECTS) | build
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

# Build *.o from *.c.
build/obj/%.o: %.c | build
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

# Build *.o from *.s.
build/obj/%.o: %.s | build
	arm-none-eabi-as $< -o $@

# Build *.list from *.o.
build/list/%.list: build/obj/%.o | build
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

# Create build directory and its subdirectories.
build:
	mkdir -p build/obj build/elf build/bin build/list
	
# Build and run the application binary.
install: $(APPLICATION) | build
	rpi-install.py -p $<

# Build and run the test binary.
test: $(TEST) | build
	rpi-install.py -p $<

# Remove the build directory (i.e. all the binary files).
clean:
	rm -rf build

# Identify targets that don't create a file.
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: all clean install test %.bin %.elf %.list %.o

# Prevent make from removing intermediate build artifacts.
.PRECIOUS: build/bin/%.bin build/elf/%.elf build/list/%.list build/obj/%.o

# Disable all built-in rules.
# https://www.gnu.org/software/make/manual/html_node/Suffix-Rules.html
.SUFFIXES:

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Review instructions for properly configuring your shell.
https://cs107e.github.io/guides/install/userconfig#env

endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif
