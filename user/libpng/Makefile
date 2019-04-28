SHARED_LIB = libpng16.so
STATIC_LIB = libpng16.a

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib

CC = $(WOOT_TOP_DIR)/user/cc

export CC CFLAGS LDFLAGS

CFLAGS += -fPIC -Wl,-soname=$(SHARED_LIB)
LDFLAGS = -Wl,-soname=$(SHARED_LIB)

export WOOT_TOP_DIR

all: configure lib/$(SHARED_LIB)

static :lib/$(STATIC_LIB)

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
	LIBS="-lc" \
	SONAME=$(SHARED_LIB) \
	../libpng-1.6.36/configure --prefix="$(WOOT_TOP_DIR)/user/libpng" \
	--host=x86_64-linux-gnu

install: $(LIB_DIR)/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB).16

$(LIB_DIR)/$(SHARED_LIB): lib/$(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB).16: lib/$(SHARED_LIB).16
	mkdir -p $(LIB_DIR)
	cp lib/$(SHARED_LIB).16 $(LIB_DIR)/$(SHARED_LIB).16

distclean:
	-rm -rf build bin lib share include
	
clean:
	-rm -rf bin lib share include
	$(MAKE) -C build clean

.PHONY: clean install reconfigure distclean

