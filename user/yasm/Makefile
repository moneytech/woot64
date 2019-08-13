WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
OUTFILE = yasm
BUILDFILE = build/$(OUTFILE)
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc

export WOOT_TOP_DIR CC CFLAGS LDFLAGS

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
	../yasm-1.3.0/configure --prefix=$(PREFIX) \
	--host=x86_64-linux-gnu

install: $(BIN_DIR)/$(OUTFILE)

$(BIN_DIR)/$(OUTFILE): $(BUILDFILE)
	mkdir -p $(BIN_DIR)
	cp $(BUILDFILE) $(BIN_DIR)/$(OUTFILE)

distclean:
	rm -rf build
	
clean:
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

