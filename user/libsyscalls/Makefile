SHARED_LIB = libsyscalls.so
STATIC_LIB = libsyscalls.a
OBJS = syscalls.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
LD = $(TOOLS_PREFIX)ld
AR = $(TOOLS_PREFIX)ar

CFLAGS = -I.
LDFLAGS = -shared -Wl,-shared,--no-undefined

export WOOT_TOP_DIR

HEADERS = $(PREFIX)/include/syscalls/syscalls.h

all: $(SHARED_LIB) $(STATIC_LIB) $(PREFIX)/lib/$(STATIC_LIB) $(PREFIX)/lib/$(SHARED_LIB) $(HEADERS)

$(PREFIX)/include/syscalls/syscalls.h: syscalls/syscalls.h

$(PREFIX)/include/syscalls/%:
	mkdir -p $(PREFIX)/include/syscalls
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

$(PREFIX)/lib/$(STATIC_LIB) $(PREFIX)/lib/$(SHARED_LIB): $(STATIC_LIB) $(SHARED_LIB)
	mkdir -p $(PREFIX)/lib
	cp $(STATIC_LIB) $(SHARED_LIB) $(PREFIX)/lib/

clean:
	rm -rf $(OBJS) $(STATIC_LIB) $(SHARED_LIB) $(PREFIX)/lib/$(SHARED_LIB) $(PREFIX)/lib/$(STATIC_LIB) $(HEADERS)

.PHONY: clean install
