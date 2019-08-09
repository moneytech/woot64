WOOT_TOP_DIR ?= $(shell realpath ../..)
SRC_DIR = $(WOOT_TOP_DIR)/user/libunwind/libunwind-master
BUILD_DIR = $(WOOT_TOP_DIR)/user/libunwind/build
CONFIG_STATUS = $(BUILD_DIR)/config.status
CONFIG_OPTS = --prefix=$(WOOT_TOP_DIR)/user/libunwind \
	--disable-tests --disable-shared --host=x86_64-woot

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
LIBS = -lc

CFLAGS = -ggdb -O0

export WOOT_TOP_DIR CC CXX LDFLAGS LIBS CFLAGS

all: configure
	$(MAKE) -C build
	$(MAKE) -C build install
	
preconfigure: $(SRC_DIR)/configure

$(SRC_DIR)/configure: $(SRC_DIR)/configure.ac $(SRC_DIR)/Makefile.am
	cd $(BUILD_DIR) && \
	NOCONFIGURE=1 \
	$(SRC_DIR)/autogen.sh
	
configure: $(CONFIG_STATUS)

$(CONFIG_STATUS): $(SRC_DIR)/configure
	mkdir -p $(BUILD_DIR) && \
	cd $(BUILD_DIR) && \
	$(SRC_DIR)/configure $(CONFIG_OPTS)

install:
	true

clean:
	cd $(BUILD_DIR) && \
	$(MAKE) -C build clean

distclean:
	rm -rf build include lib

.PHONY: preconfigure configure clean distclean install
