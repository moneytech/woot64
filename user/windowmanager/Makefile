OUTFILE = windowmanager
OUTFILE_STATIC = windowmanager_static
OBJS = cxx.o window.o wm.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(WOOT_TOP_DIR)/root/bin
CUR_FILE = $(WOOT_TOP_DIR)/root/normal.cur
DEFAULT_TTF_FILE = $(WOOT_TOP_DIR)/root/default.ttf
TITLE_TTF_FILE = $(WOOT_TOP_DIR)/root/title.ttf

CC = $(WOOT_TOP_DIR)/user/cc
CXX = $(WOOT_TOP_DIR)/user/cxx
CFLAGS = -ggdb
CXXFLAGS = -ggdb
LIBS = \
	$(WOOT_TOP_DIR)/user/libc/lib/crt1.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crti.o \
	$(WOOT_TOP_DIR)/user/libc/lib/crtn.o \
	-Wl,--start-group,-lc,-lwoot,-lsyscalls,-lpng,-lz,-lfreetype,--end-group

export WOOT_TOP_DIR

all: $(OUTFILE)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CC) -static $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

install: $(BIN_DIR)/$(OUTFILE) $(CUR_FILE) $(DEFAULT_TTF_FILE) $(TITLE_TTF_FILE)

install-static: $(BIN_DIR)/$(OUTFILE_STATIC) $(CUR_FILE) $(DEFAULT_TTF_FILE) $(TITLE_TTF_FILE)

$(BIN_DIR)/$(OUTFILE): $(OUTFILE)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

$(BIN_DIR)/$(OUTFILE_STATIC): $(OUTFILE_STATIC)
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE_STATIC) $(BIN_DIR)/$(OUTFILE_STATIC)

$(CUR_FILE): normal.cur
	cp $? $@

$(DEFAULT_TTF_FILE): default.ttf
	cp $? $@

$(TITLE_TTF_FILE): title.ttf
	cp $? $@

clean:
	rm -f $(OBJS) $(OUTFILE_STATIC) $(OUTFILE)

.PHONY: clean install static install-static
