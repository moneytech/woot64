WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
OUTFILE = yasm
BUILDFILE = build/$(OUTFILE)

CC = $(WOOT_TOP_DIR)/user/cc
LDFLAGS = \
	$(WOOT_TOP_DIR)/user/libc/lib/crt1.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crti.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crtn.o \
	-lc

export CC CFLAGS LDFLAGS WOOT_TOP_DIR

all: $(BUILDFILE)

$(BUILDFILE):
	$(MAKE) configure
	$(MAKE) $(OUTFILE)

$(OUTFILE):
	$(MAKE) -C build

configure: build/config.status

reconfigure:
	rm -f build/config.status
	$(MAKE) configure

build/config.status:
	mkdir -p build
	cd build && \
	rm -rf ./* && \
	../yasm-1.3.0/configure --prefix="$(WOOT_TOP_DIR)/user/yasm" \
	--host=x86_64-linux-gnu

install: $(BIN_DIR)/$(OUTFILE)

$(BIN_DIR)/$(OUTFILE): $(BUILDFILE)
	mkdir -p $(BIN_DIR)
	cp $(BUILDFILE) $(BIN_DIR)/$(OUTFILE)

distclean:
	rm -rf build bin lib share include
	
clean:
	rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

