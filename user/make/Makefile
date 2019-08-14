WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
PREFIX = $(WOOT_TOP_DIR)/root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
CXX = $(TOOLS_PREFIX)g++

export WOOT_TOP_DIR CC CXX LD CFLAGS LDFLAGS

SOURCE_DIR = make-4.2.1
CONFIGURE_SCRIPT = ../$(SOURCE_DIR)/configure
CONFIGURE_OPTS = --prefix=$(PREFIX) --host=x86_64-woot --without-guile --disable-nls

all:
	$(MAKE) configure
	$(MAKE) -C build

configure: build/config.status

build/config.status:
	mkdir -p build
	cd build && \
	rm -rf ./* && \
	$(CONFIGURE_SCRIPT) $(CONFIGURE_OPTS)

install:
	$(MAKE) -C build install

uninstall:
	$(MAKE) -C build uninstall

clean:
	-$(MAKE) -C build clean

distclean:
	-$(MAKE) -C build uninstall
	rm -rf build

.PHONY: clean install uninstall distclean

