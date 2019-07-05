WOOT_TOP_DIR ?= $(shell realpath ../..)
SRC_DIR = $(WOOT_TOP_DIR)/user/libunwind/libunwind-master
BUILD_DIR = $(WOOT_TOP_DIR)/user/libunwind/build
BUILD_PLACEHOLDER = $(BUILD_DIR)/placeholder
CONFIG_STATUS = $(BUILD_DIR)/config.status
CONFIG_OPTS = --prefix=$(WOOT_TOP_DIR)/user/libunwind \
	--disable-tests --disable-shared --host=x86_64-linux

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
LIBS = -lc

export WOOT_TOP_DIR CC CXX LDFLAGS LIBS

all: configure
	$(MAKE) -C build
	$(MAKE) -C build install
	
$(BUILD_PLACEHOLDER):
	mkdir -p $(BUILD_DIR) && touch $@

preconfigure: $(SRC_DIR)/configure

$(SRC_DIR)/configure: $(BUILD_PLACEHOLDER) $(SRC_DIR)/configure.ac $(SRC_DIR)/Makefile.am
	cd $(BUILD_DIR) && \
	$(SRC_DIR)/autogen.sh
	
configure: $(CONFIG_STATUS)

$(CONFIG_STATUS): $(SRC_DIR)/configure
	cd $(BUILD_DIR) && \
	$(SRC_DIR)/configure $(CONFIG_OPTS)

install:
	true

clean:
	cd $(BUILD_DIR) && \
	$(MAKE) -C build clean

distclean:
	rm -rf build

.PHONY: preconfigure configure clean distclean install
