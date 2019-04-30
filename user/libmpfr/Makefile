SHARED_LIB = libmpfr.so
STATIC_LIB = libmpfr.a

SHARED_VERSIONED = $(SHARED_LIB).6

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
	../mpfr-4.0.2/configure \
	--prefix="$(WOOT_TOP_DIR)/user/libmpfr" \
	--host=x86_64-linux

install: $(LIB_DIR)/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_VERSIONED)

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_VERSIONED): lib/$(SHARED_VERSIONED)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_VERSIONED) $(LIB_DIR)/$(SHARED_VERSIONED)

distclean:
	-rm -rf build bin lib share include
	
clean:
	-rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

