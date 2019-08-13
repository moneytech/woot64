OUTFILE = console
OUTFILE_STATIC = console_static
OBJS = console.o font.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
CXX = $(TOOLS_PREFIX)g++

CFLAGS = -ggdb
CXXFLAGS = -ggdb
LDFLAGS =
LIBS = -Wl,--start-group,-lgcc_eh,-lwoot,-lsyscalls,--end-group

export WOOT_TOP_DIR CC CXX

all: $(OUTFILE)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CXX) -static $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

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
