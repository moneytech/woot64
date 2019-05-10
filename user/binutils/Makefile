SRC_DIR = binutils-2.32
WOOT_TOP_DIR ?= $(shell realpath ../..)

export WOOT_TOP_DIR

all: configure

cross: configure-cross
	$(MAKE) -C build-cross all
	$(MAKE) -C build-cross install

configure-help:
	mkdir -p build
	cd build && ../$(SRC_DIR)/configure --help

configure: build/config.status

configure-cross: build-cross/config.status

build-cross/config.status:
	mkdir -p build-cross
	cd build-cross && ../$(SRC_DIR)/configure \
	--prefix=$(WOOT_TOP_DIR)/user/cross-tools \
	--with-sysroot=$(WOOT_TOP_DIR)/user/cross-root \
	--target=x86_64-woot

reconfigure:
	rm -rf build
	$(MAKE) configure

reconfigure-cross:
	rm -rf build-cross
	$(MAKE) configure-cross

clean:
	$(MAKE) -C build clean

clean-cross:
	$(MAKE) -C build-cross clean

distclean:
	rm -rf build build-cross

.PHONY: configure-help reconfigure reconfigure-cross clean clean-cross distclean

