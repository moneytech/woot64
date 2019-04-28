WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
CC = clang

SHARED_LIB = libc.so
STATIC_LIB = libc.a
SHARED_LIB_PATH = lib/$(SHARED_LIB)
STATIC_LIB_PATH = lib/$(STATIC_LIB)

export WOOT_TOP_DIR CC

all: $(SHARED_LIB_PATH)

static: $(STATIC_LIB_PATH)

$(SHARED_LIB_PATH) $(STATIC_LIB_PATH): build/config.mak
	$(MAKE) -C build
	$(MAKE) -C build install

configure: build/config.mak

build/config.mak:
	mkdir -p build
	cd build && \
	CFLAGS='-D__WOOT__ -nostdinc -fno-omit-frame-pointer -O0 -ggdb' \
	LDFLAGS="-Wl,-z,max-page-size=4096" \
	../musl-1.1.21/configure --prefix=$(WOOT_TOP_DIR)/user/libc --host=x86_64-linux-gnu


reconfigure:
	rm -rf build
	$(MAKE) configure

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB_PATH)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB_PATH) $(LIB_DIR)/$(SHARED_LIB)

distclean:
	rm -rf build include lib

clean:
	$(MAKE) -C build clean
	rm -rf include lib

.PHONY: clean distclean install reconfigure $(SHARED_LIB_PATH) $(STATIC_LIB_PATH)

