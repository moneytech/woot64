SHARED_LIB = libz.so
STATIC_LIB = libz.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib

CC = $(WOOT_TOP_DIR)/user/cc

export CC CFLAGS LDFLAGS

CFLAGS += -fPIC -Wno-undefined-internal -lc
LDFLAGS = -shared -Wl,-soname=$(SHARED_LIB)

export WOOT_TOP_DIR

all: configure lib/$(SHARED_LIB)

static: lib/$(STATIC_LIB)

lib/$(SHARED_LIB) lib/$(STATIC_LIB):
	$(MAKE) -C build
	$(MAKE) -C build install

configure: build/configure.log

reconfigure:
	-rm build/configure.log
	$(MAKE) configure

build/configure.log:
	mkdir -p build
	cd build && \
	rm -rf ./* && \
	SONAME=libz.so \
	LDSHAREDLIBC="-nostdlib -Wl,-L$(WOOT_TOP_DIR)/user/lib -lc -lgcc" \
	../zlib-1.2.11/configure --prefix="$(WOOT_TOP_DIR)/user/libz"

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	-rm -rf build lib share include
	
clean:
	-rm -rf lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean static

