OUTFILE = kernel
OBJS = boot/boot.o boot/boot64.o \
	bitmap.o \
	callbackstream.o \
	character.o \
	cmos.o \
	cpu.o \
	cxx.o \
	debug.o \
	heap.o \
	ints.o \
	irqs.o \
	isrs.o \
	kmain.o \
	memory.o \
	misc.o \
	paging.o \
	random.o \
	stream.o \
	string.o \
	time.o \
	uuid.o

TOP_DIR ?= $(shell realpath ..)
CC ?= clang
CXX ?= clang++
ASM ?= yasm
LD ?= ld

COMPILE_FLAGS = \
	-ffreestanding \
	-mcmodel=large \
	-mno-red-zone \
	-O0 \
	-fno-stack-protector \
	-fshort-wchar \
	-Ulinux \
	-D__WOOT__ \
	-ggdb \
	-I$(TOP_DIR)/kernel/include \
	-masm=intel
	
CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions -fno-rtti
ASMFLAGS ?= -f elf64 -g dwarf2 -w
LDFLAGS = -Tlink.ld -z max-page-size=4096

DEPS = link.ld

all: $(OUTFILE)

$(OUTFILE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

$(TOP_DIR)/root/boot/grub/grub.cfg: boot/grub/grub.cfg
	mkdir -p $(TOP_DIR)/root/boot/grub
	cp $< $@

$(TOP_DIR)/root/system/$(OUTFILE): $(OUTFILE)
	mkdir -p $(TOP_DIR)/root/system
	cp $(OUTFILE) $(TOP_DIR)/root/system/$(OUTFILE)

install: $(TOP_DIR)/root/boot/grub/grub.cfg $(TOP_DIR)/root/system/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: clean install

