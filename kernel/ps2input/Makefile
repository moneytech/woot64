WOOT_TOP_DIR ?= $(shell realpath ../..)
CUR_DIR = $(shell realpath .)

OUTFILE = ps2input
MODULE_FILE = $(WOOT_TOP_DIR)/root/system/$(OUTFILE)

OBJS = module.o ps2.o ps2kbd.o ps2mouse.o

CC ?= clang
CXX ?= clang++
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
	-I$(WOOT_TOP_DIR)/kernel/include \
	-I$(CUR_DIR) \
	-shared

CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions
LDFLAGS = \
	-z max-page-size=4096 \
	-T$(WOOT_TOP_DIR)/kernel/module.ld \
	--section-start .headers=0xFFFFF80020000000 \
	-shared

all: $(OUTFILE)

$(OUTFILE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

clean:
	rm -rf $(OBJS) $(OUTFILE)

install: $(MODULE_FILE)

$(MODULE_FILE): $(OUTFILE)
	cp $? $@

.PHONY: clean install

