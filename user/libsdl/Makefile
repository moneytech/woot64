SHARED_LIB = libSDL.so
STATIC_LIB = libSDL.a

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
	../SDL-1.2.15/configure \
	--prefix="$(WOOT_TOP_DIR)/user/libfreetype" \
	--host=x86_64-woot

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	-rm -rf build bin lib share include
	
clean:
	-rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

