WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = # we use native gcc to build libc
CC = $(TOOLS_PREFIX)gcc
AR = $(TOOLS_PREFIX)ar
AS = $(TOOLS_PREFIX)as
LD = $(TOOLS_PREFIX)ld
RANLIB = $(TOOLS_PREFIX)ranlib

SHARED_LIB = libc.so
STATIC_LIB = libc.a
SHARED_LIB_PATH = $(PREFIX)/lib/$(SHARED_LIB)
STATIC_LIB_PATH = $(PREFIX)/lib/$(STATIC_LIB)

DISABLE_LDSO = yes

export WOOT_TOP_DIR CC AR AS LD RANLIB DISABLE_LDSO

all: $(SHARED_LIB_PATH)

static: $(STATIC_LIB_PATH)

$(SHARED_LIB_PATH) $(STATIC_LIB_PATH): build/config.mak
	$(MAKE) -C build
	$(MAKE) -C build install

configure: build/config.mak

build/config.mak:
	mkdir -p build
	cd build && \
	CFLAGS='-D__WOOT__ -Dlinux -D__linux__ -nostdinc -fno-omit-frame-pointer -O0 -ggdb' \
	LDFLAGS="-Wl,--no-gc-sections" \
	../musl-1.1.21/configure --prefix=$(PREFIX)

reconfigure:
	rm -rf build
	$(MAKE) configure

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB_PATH)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB_PATH) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	rm -rf build

clean:
	$(MAKE) -C build clean

.PHONY: clean distclean install reconfigure $(SHARED_LIB_PATH) $(STATIC_LIB_PATH)

