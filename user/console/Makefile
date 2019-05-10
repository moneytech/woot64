OUTFILE = console
OUTFILE_STATIC = console_static
OBJS = console.o font.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
CFLAGS = -ggdb
CXXFLAGS = -ggdb
LIBS = \
	$(WOOT_TOP_DIR)/user/libc/lib/Scrt1.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crti.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crtn.o \
	-Wl,--start-group,-lc,-lwoot,-lsyscalls,--end-group

export WOOT_TOP_DIR

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
