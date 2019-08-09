OUTFILE = usertest
OUTFILE_STATIC = $(OUTFILE)_static
OBJS = usertest.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
AR = $(TOOLS_PREFIX)ar
LD = $(TOOLS_PREFIX)ld
LIBS =
#	$(WOOT_TOP_DIR)/user/libc/lib/Scrt1.o \
#	$(WOOT_TOP_DIR)/user/libc/lib/crti.o \
#	$(WOOT_TOP_DIR)/user/libc/lib/crtn.o \
#	-Wl,--start-group,-lc,-lwoot,-lsyscalls,-lz,-lpng16,-lfreetype,-lSDL,-lunwind,-lunwind-generic,-llzma,--end-group

CFLAGS = -ggdb -O0
LDFLAGS =

export WOOT_TOP_DIR CC AR LD CFLAGS LDFLAGS

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
