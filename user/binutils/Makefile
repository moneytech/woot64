WOOT_ROOT_DIR ?= $(shell realpath ../..)
SRC_DIR = binutils-2.32

export WOOT_ROOT_DIR

all: configure

build:
	mkdir -p build

build-cross:
	mkdir -p build-cross

configure-help: build
	cd build && ../$(SRC_DIR)/configure --help

configure: build/config.status

configure-cross: build-cross/config.status

build-cross/config.status: build-cross

reconfigure:
	rm -rf build
	$(MAKE) configure

reconfigure-cross:
	rm -rf build-cross
	$(MAKE) configure-cross

clean:

distclean:
	rm -rf build build-cross

.PHONY: configure-help reconfigure reconfigure-cross clean distclean

