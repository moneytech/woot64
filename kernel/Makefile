OUTFILE = kernel
MODULES = ps2input testmodule
OBJS = boot/boot.o boot/boot64.o \
	ahci/ahcidrive.o \
	bitmap.o \
	bufferedvolume.o \
	callbackstream.o \
	character.o \
	cmos.o \
	cpu.o \
	cxx.o \
	cxxabi.o \
	debug.o \
	dentry.o \
	device.o \
	directoryentry.o \
	drive.o \
	elf.o \
	ext2/ext2.o \
	fbfont.o \
	file.o \
	filestream.o \
	filesystem.o \
	filesystemtype.o \
	heap.o \
	ide/idedrive.o \
	inode.o \
	inputdevice.o \
	ints.o \
	irqs.o \
	isrs.o \
	kmain.o \
	memory.o \
	memorystream.o \
	misc.o \
	module.o \
	mutex.o \
	namedobject.o \
	paging.o \
	partvolume.o \
	pci.o \
	process.o \
	random.o \
	semaphore.o \
	sharedmem.o \
	stream.o \
	string.o \
	stringbuilder.o \
	syscalls.o \
	thread.o \
	time.o \
	tokenizer.o \
	typeinfo.o \
	uuid.o \
	volume.o \
	volumetype.o 

TOP_DIR ?= $(shell realpath ..)
CC ?= clang
CXX ?= clang++
ASM ?= yasm
LD ?= ld

COMPILE_FLAGS = \
	-masm=intel \
	-ffreestanding \
	-nostdinc \
	-mcmodel=large \
	-mno-red-zone \
	-O0 \
	-fno-stack-protector \
	-fshort-wchar \
	-Ulinux \
	-D__WOOT__ \
	-ggdb \
	-I$(TOP_DIR)/kernel/include \
	-fno-pic
		
CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions
ASMFLAGS = -f elf64 -g dwarf2 -w
LDFLAGS = -Tlink.ld -z max-page-size=4096 -shared -Bsymbolic -z noreloc-overflow

DEPS = link.ld

all: $(OUTFILE)
	for dir in $(MODULES); do $(MAKE) -C $$dir; done

$(OUTFILE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

$(TOP_DIR)/root/boot/grub/grub.cfg: boot/grub/grub.cfg
	mkdir -p $(TOP_DIR)/root/boot/grub
	cp $< $@

$(TOP_DIR)/root/system/$(OUTFILE): $(OUTFILE)
	mkdir -p $(TOP_DIR)/root/system
	cp $(OUTFILE) $(TOP_DIR)/root/system/$(OUTFILE)

$(TOP_DIR)/root/system/modulelist: modulelist
	mkdir -p $(TOP_DIR)/root/system
	cp $< $@

install: $(TOP_DIR)/root/boot/grub/grub.cfg $(TOP_DIR)/root/system/$(OUTFILE) $(TOP_DIR)/root/system/modulelist
	for dir in $(MODULES); do $(MAKE) -C $$dir install; done

clean:
	rm -f $(OBJS) $(OUTFILE)

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: clean install

