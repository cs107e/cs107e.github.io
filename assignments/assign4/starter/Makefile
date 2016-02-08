#
# A simple makefile for building program composed of C/asm source files.
# Builds for ARM target using cross-development toolchain.
#

# Set variables to correct tools
ARMGNU = arm-none-eabi
CC = $(ARMGNU)-gcc
AS = $(ARMGNU)-as
OBJCOPY = $(ARMGNU)-objcopy
OBJDUMP = $(ARMGNU)-objdump

# Configure compile options via CFLAGS
#  -gstabs			compile with STABS debug information
#  -Wall     		give verbose compiler warnings
#  -Og       		apply moderate optimization, retain structure for debugging
#  -std=gnu99  		use the C99 standard language definition with GNU extenions
#  -ffreestanding	do not assume presence of standard library (non-hosted)
#  -mapcs-frame		always emit a complete stack frame
#  -fno-omit-frame-pointer	never optimize away the frame pointer
CFLAGS = -gstabs -Wall -Og -std=gnu99 -ffreestanding -Iinc/ -mapcs-frame \
		 -fno-omit-frame-pointer -DREDZONE_MALLOC

# Configure linker options via LDFLAGS, LDLIBS
#  -nostdlib   	do not link standard system startup nor default libraries
#  -T memmap    use our linker map
LDFLAGS = -nostdlib -T memmap
LDLIBS = -Llibs/ -lpi -lgcc

# --gstabs 			assemble with STABS debug information
ASFLAGS = --gstabs

# Set name of the executable, built from single source file 'name.c'
# Edit here to change executable name or add/remove/change source files
NAME = main
SOURCES = $(NAME).c
OBJECTS = $(SOURCES:.c=.o)

define objcopy_sect_flags
	-O binary --set-section-flags $(1)=alloc -j $(1)
endef

define prepend_size_to
	./libs/get_bytes.sh $$(wc -c $(1) | awk '{print $$1}') | cat - $(1) > $(1).tmp
	mv $(1).tmp $(1)
endef

# The first target listed in the makefile is the default product built when
# make is invoked with no argument
all: $(NAME).bin

# Link object files into elf executable, use custom start.o in place of default
$(NAME).elf: start.o $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# The install target uploads freshly made binary image to rpi bootloader
install: $(NAME).bin
	rpi-install.py $<

# This pattern rule creates binary 'name.bin' by extracting raw instructions
# from executable 'name.elf'
%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.bin.extra: %.elf
	$(OBJCOPY) $< $(call objcopy_sect_flags,.stab) $*.stab
	$(call prepend_size_to,$*.stab)
	$(OBJCOPY) $< $(call objcopy_sect_flags,.stabstr) $*.stabstr
	$(call prepend_size_to,$*.stabstr)
	$(OBJCOPY) $< -O binary $*.text
	cat $*.text $*.stab $*.stabstr > $@

# This pattern rule produces 'name.list' assembly listing by disassembling 'name.o'
%.list: %.o
	$(OBJDUMP) -d $< > $@

# These 2 pattern rules not truly necessary, just make explicit what was implicit...
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# The clean target removes previous build products, will force build a-new on next make
clean:
	rm -f *.o *.bin *.elf *.list *.text *.stab *.stabstr *.bin.extra

# Targets that aren't filename of build product should be declared "phony"
.PHONY: all clean

