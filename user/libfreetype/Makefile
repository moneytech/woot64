SHARED_LIB = libfreetype.so
STATIC_LIB = libfreetype.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib

CC = $(WOOT_TOP_DIR)/user/cc

export CC CFLAGS LDFLAGS

CFLAGS = -lc
LDFLAGS = 

export WOOT_TOP_DIR

all: configure lib/$(SHARED_LIB)

static: lib/$(STATIC_LIB)

lib/$(SHARED_LIB) lib/$(STATIC_LIB):
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
	LIBPNG_CFLAGS="-I$(WOOT_TOP_DIR)/user/libpng/include" \
	LIBPNG_LIBS="-L$(WOOT_TOP_DIR)/user/libpng/lib -lpng16" \
	../freetype-2.10.0/configure \
	--prefix="$(WOOT_TOP_DIR)/user/libfreetype" \
	--host=x86_64-linux

install: $(LIB_DIR)/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB).6

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB).6: lib/$(SHARED_LIB).6
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB).6 $(LIB_DIR)/$(SHARED_LIB).6

distclean:
	-rm -rf build bin lib share include
	
clean:
	-rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

