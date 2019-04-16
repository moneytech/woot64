OUTFILE = inputhandler
OUTFILE_STATIC = inputhandler_static
OBJS = inputhandler.o

TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(TOP_DIR)/root/bin
CUR_FILE = $(TOP_DIR)/root/normal.cur

CC = $(TOP_DIR)/user/cc
CXX = $(TOP_DIR)/user/cxx
LIBS = \
	$(TOP_DIR)/user/libc/lib/crt1.o \
	$(TOP_DIR)/user/libc/lib/crti.o \
	$(TOP_DIR)/user/libc/lib/crtn.o \
	-Wl,--start-group,-lc,-lwoot,-lsyscalls,--end-group

export TOP_DIR

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
