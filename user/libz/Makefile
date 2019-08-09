SHARED_LIB = libz.so
STATIC_LIB = libz.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc

CFLAGS = -fPIC -Wno-undefined-internal -lc
LDFLAGS = -Wl,-soname=$(SHARED_LIB)

export WOOT_TOP_DIR CC CFLAGS LDFLAGS

all: configure
	$(MAKE) lib/$(SHARED_LIB)

static: lib/$(STATIC_LIB)

lib/$(SHARED_LIB) lib/$(STATIC_LIB):
	$(MAKE) -C build
	$(MAKE) -C build install

configure: build/configure.log

reconfigure:
	rm -rf build
	$(MAKE) configure

build/configure.log:
	mkdir -p build
	cd build && \
	rm -rf ./* && \
	SONAME=libz.so \
	../zlib-1.2.11/configure --prefix=$(PREFIX)

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	rm -rf build lib share include
	
clean:
	rm -rf lib share include
	$(MAKE) -C build clean

.PHONY: clean install configure reconfigure distclean static

