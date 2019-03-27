OUTFILE = kernel
OBJS = boot/boot.o

TOP_DIR ?= $(shell realpath ..)
CC ?= clang
CXX ?= clang++
ASM ?= yasm
LD ?= ld

COMPILE_FLAGS = -O0 -nostdinc -fno-stack-protector -fshort-wchar -D__WOOT__ -ggdb -I$(TOP_DIR)/kernel
CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions -fno-rtti
ASMFLAGS ?= -f elf64 -g dwarf2 -w
LDFLAGS = -Tlink.ld

DEPS = link.ld

all: $(OUTFILE)

$(OUTFILE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

install:
	mkdir -p $(TOP_DIR)/system
	cp $(TOP_DIR)/system/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

.PHONY: clean install

