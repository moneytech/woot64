OUTFILE = inputhandler
OUTFILE_STATIC = inputhandler_static
OBJS = inputhandler.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
CXX = $(TOOLS_PREFIX)g++
LIBS = -Wl,--start-group,-lwoot,-lsyscalls,--end-group

export WOOT_TOP_DIR CC CXX LD CFLAGS LDFLAGS

all: $(OUTFILE)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CC) -static $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

install: $(BIN_DIR)/$(OUTFILE)

install-static: $(BIN_DIR)/$(OUTFILE_STATIC)

$(BIN_DIR)/$(OUTFILE): $(OUTFILE)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

$(BIN_DIR)/$(OUTFILE_STATIC): $(OUTFILE_STATIC)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE_STATIC) $(BIN_DIR)/$(OUTFILE_STATIC)

clean:
	rm -f $(OBJS) $(OUTFILE_STATIC) $(OUTFILE)

.PHONY: clean install static install-static
