SHARED_LIB = libSDL.so
STATIC_LIB = libSDL.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc

export WOOT_TOP_DIR CC CFLAGS LDFLAGS

all:
	$(MAKE) configure
	$(MAKE) $(PREFIX)/lib/$(SHARED_LIB)

static: $(PREFIX)/lib/$(STATIC_LIB)

$(PREFIX)/lib/$(SHARED_LIB) $(PREFIX)/lib/$(STATIC_LIB):
	$(MAKE) -C build
	$(MAKE) -C build install

configure: build/config.status

reconfigure:
	-rm build/config.status
	$(MAKE) configure

build/config.status:
	mkdir -p build
	cd build && \
	rm -rf ./* && \
	../SDL-1.2.15/configure \
	--prefix=$(PREFIX) \
	--host=x86_64-woot

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): $(PREFIX)/lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(PREFIX)/lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	-$(MAKE) -C build uninstall
	-rm -rf build
	
clean:
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

