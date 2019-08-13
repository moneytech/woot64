SHARED_LIB = libfreetype.so
STATIC_LIB = libfreetype.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc

export WOOT_TOP_DIR CC

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
	PKG_CONFIG="false" \
	SONAME="$(SHARED_LIB)" \
	LIBPNG_CFLAGS="-I$(PREFIX)/include" \
	LIBPNG_LIBS="-L$(PREFIX)/lib -lpng16" \
	CFLAGS="-fPIC" \
	../freetype-2.10.0/configure \
	--prefix=$(PREFIX) \
	--host=x86_64-woot

install: $(LIB_DIR)/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB).6

$(LIB_DIR)/$(SHARED_LIB): $(PREFIX)/lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(PREFIX)/lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB).6: $(WOOT_TOP_DIR)/cross-root/lib/$(SHARED_LIB).6
	mkdir -p $(LIB_DIR)
	cp $(PREFIX)/lib/$(SHARED_LIB).6 $(LIB_DIR)/$(SHARED_LIB).6

distclean:
	-$(MAKE) -C build uninstall
	-rm -rf build bin lib share include
	
clean:
	-rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

