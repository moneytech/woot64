OUTFILE = windowmanager
OUTFILE_STATIC = windowmanager_static
OBJS = cxx.o window.o wm.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
CUR_FILE = $(WOOT_TOP_DIR)/root/normal.cur

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
LIBS = \
	$(WOOT_TOP_DIR)/user/libc/lib/crt1.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crti.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crtn.o \
	-Wl,--start-group,-lc,-lwoot,-lsyscalls,-lpng,-lz,--end-group

export WOOT_TOP_DIR

all: $(OUTFILE) $(OUTFILE_STATIC)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CC) -static $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

install: $(BIN_DIR)/$(OUTFILE) $(BIN_DIR)/$(OUTFILE_STATIC) $(CUR_FILE)

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

.PHONY: clean install static
