OUTFILE = windowmanager
OUTFILE_STATIC = windowmanager_static
OBJS = cxx.o window.o wm.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
CUR_FILE = $(WOOT_TOP_DIR)/root/normal.cur
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
CXX = $(TOOLS_PREFIX)g++
LIBS = -Wl,--start-group,-lgcc,-lstdc++,-lwoot,-lsyscalls,-lpng,-lz,-lfreetype,--end-group

CFLAGS = -ggdb
CXXFLAGS = -ggdb

export WOOT_TOP_DIR CC CXX CFLAGS CXXFLAGS

all: $(OUTFILE)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CXX) -static $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

install: $(BIN_DIR)/$(OUTFILE) $(CUR_FILE)

install-static: $(BIN_DIR)/$(OUTFILE_STATIC) $(CUR_FILE)

$(BIN_DIR)/$(OUTFILE): $(OUTFILE)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

$(BIN_DIR)/$(OUTFILE_STATIC): $(OUTFILE_STATIC)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE_STATIC) $(BIN_DIR)/$(OUTFILE_STATIC)

$(CUR_FILE): normal.cur
	cp $? $@

clean:
	rm -f $(OBJS) $(OUTFILE_STATIC) $(OUTFILE)

.PHONY: clean install static install-static
