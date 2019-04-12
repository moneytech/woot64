OUTFILE = mbootfb
MODULE_FILE = $(TOP_DIR)/root/system/$(OUTFILE)

OBJS = module.o mbootfb.o

TOP_DIR ?= $(shell realpath ../..)
CUR_DIR = $(shell realpath .)

CC ?= clang
CXX ?= clang++

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
	-I$(CUR_DIR) \
	-shared

CFLAGS = $(COMPILE_FLAGS)
CXXFLAGS = $(COMPILE_FLAGS) -fno-exceptions
LDFLAGS = \
	-z max-page-size=4096 \
	-T$(TOP_DIR)/kernel/module.ld \
	--section-start .headers=0xFFFFF80028000000 \
	-shared

all: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

clean:
	rm -rf $(OBJS) $(OUTFILE)

install: $(MODULE_FILE)

$(MODULE_FILE): $(OUTFILE)
	cp $? $@

.PHONY: clean install


