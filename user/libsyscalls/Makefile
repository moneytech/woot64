SHARED_LIB = libsyscalls.so
STATIC_LIB = libsyscalls.a
OBJS = syscalls.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
INSTALL_DIR = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
LD = $(TOOLS_PREFIX)ld
AR = $(TOOLS_PREFIX)ar

CFLAGS = -I.
LDFLAGS = -shared -nostdlib -Wl,--no-undefined

export WOOT_TOP_DIR CC LD AR CFLAGS LDFLAGS

HEADERS = $(INSTALL_DIR)/include/syscalls/syscalls.h

$(INSTALL_DIR)/include/syscalls/syscalls.h: syscalls/syscalls.h

all: $(SHARED_LIB) $(STATIC_LIB) $(INSTALL_DIR)/lib/$(STATIC_LIB) $(INSTALL_DIR)/lib/$(SHARED_LIB) $(HEADERS)

$(INSTALL_DIR)/include/syscalls/%:
	mkdir -p $(INSTALL_DIR)/include/syscalls
	cp -f $< $@

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(SHARED_LIB)

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(INSTALL_DIR)/lib/$(STATIC_LIB) $(INSTALL_DIR)/lib/$(SHARED_LIB): $(STATIC_LIB) $(SHARED_LIB)
	mkdir -p $(INSTALL_DIR)/lib
	cp $(STATIC_LIB) $(SHARED_LIB) $(INSTALL_DIR)/lib/

clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: clean install
