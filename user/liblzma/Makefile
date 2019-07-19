WOOT_TOP_DIR ?= $(shell realpath ../..)
SRC_DIR = $(WOOT_TOP_DIR)/user/liblzma/xz-5.2.4
BUILD_DIR = $(WOOT_TOP_DIR)/user/liblzma/build
CONFIG_STATUS = $(BUILD_DIR)/config.status
CONFIG_OPTS = --prefix=$(WOOT_TOP_DIR)/user/liblzma --host=x86_64-none-linux

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
LIBS = -lc

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
	cp $(WOOT_TOP_DIR)/user/liblzma/lib/*.so* $(WOOT_TOP_DIR)/root/lib/

clean:
	cd $(BUILD_DIR) && \
	$(MAKE) -C build clean

distclean:
	rm -rf build bin include lib share

.PHONY: configure clean distclean install

