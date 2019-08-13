WOOT_TOP_DIR ?= $(shell realpath ../..)
SRC_DIR = $(WOOT_TOP_DIR)/user/liblzma/xz-5.2.4
BUILD_DIR = $(WOOT_TOP_DIR)/user/liblzma/build
CONFIG_STATUS = $(BUILD_DIR)/config.status
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-
CONFIG_OPTS = --prefix=$(PREFIX) --host=x86_64-woot

CC = $(TOOLS_PREFIX)gcc
CXX = $(TOOLS_PREFIX)g++

export WOOT_TOP_DIR CC CXX LDFLAGS LIBS

all: configure
	$(MAKE) -C build
	$(MAKE) -C build install

configure: $(CONFIG_STATUS)

$(CONFIG_STATUS): $(SRC_DIR)/configure
	mkdir -p $(BUILD_DIR) && \
	cd $(BUILD_DIR) && \
	$(SRC_DIR)/configure $(CONFIG_OPTS)

install:
	cp -R $(PREFIX)/lib/*.so* $(WOOT_TOP_DIR)/root/lib/

clean:
	$(MAKE) -C build clean

distclean:
	-$(MAKE) -C build uninstall
	rm -rf build

.PHONY: configure clean distclean install

