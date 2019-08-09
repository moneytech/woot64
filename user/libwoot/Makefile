SHARED_LIB = libwoot.so
STATIC_LIB = libwoot.a
OBJS = \
	font.o \
	input.o \
	ipc.o \
	misc.o \
	pixmap.o \
	process.o \
	rectangle.o \
	rpc.o \
	shell.o \
	signal.o \
	sync.o \
	thread.o \
	timer.o \
	ui.o \
	uibutton.o \
	uidirview.o \
	uilabel.o \
	uilineedit.o \
	uimenu.o \
	uiscrollbar.o \
	uislider.o \
	uitextedit.o \
	uitoolbar.o \
	vector.o \
	video.o \
	wm.o

WOOT_TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(WOOT_TOP_DIR)/root/lib
CFG_DIR ?= $(WOOT_TOP_DIR)/root/etc/woot
PREFIX = $(WOOT_TOP_DIR)/cross-root
TOOLS_PREFIX = $(WOOT_TOP_DIR)/cross-root/bin/x86_64-woot-

CC = $(TOOLS_PREFIX)gcc
AR = $(TOOLS_PREFIX)ar
LD = $(TOOLS_PREFIX)ld

CFLAGS = -fPIC -I. -I$(WOOT_TOP_DIR)/cross-root/include/freetype2
#LDFLAGS = -shared -Wl,-soname=$(SHARED_LIB),--no-undefined
#LIBS = -Wl,--start-group,-lsyscalls,-lpng,-lfreetype,--end-group 
LDFLAGS = -shared -soname=$(SHARED_LIB) --no-undefined
LIBS = --start-group -lc -lsyscalls -lpng -lfreetype --end-group 

export WOOT_TOP_DIR CC AR LD

HEADER_FILES = $(wildcard woot/*.h)
HEADERS = $(patsubst %, $(PREFIX)/include/%, $(HEADER_FILES))

$(HEADERS): $(HEADER_FILES)

all: $(SHARED_LIB) $(STATIC_LIB) $(HEADERS) $(PREFIX)/lib/$(STATIC_LIB) $(PREFIX)/lib/$(SHARED_LIB)

$(PREFIX)/include/%:
	mkdir -p $(PREFIX)/include/woot
	cp -f $* $@

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(SHARED_LIB) $(LIBS)

install: $(LIB_DIR)/$(SHARED_LIB) $(CFG_DIR)/editors.cfg

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(CFG_DIR)/editors.cfg: editors.cfg
	mkdir -p $(CFG_DIR)
	cp $< $@

$(PREFIX)/lib/$(STATIC_LIB) $(PREFIX)/lib/$(SHARED_LIB): $(STATIC_LIB) $(SHARED_LIB)
	mkdir -p $(PREFIX)/lib
	cp $(STATIC_LIB) $(SHARED_LIB) $(PREFIX)/lib/

pixmap.o: pixmap.c
	$(CC) $(CFLAGS) -O2 -c $< -o $@

font.o: font.c
	$(CC) $(CFLAGS) -O2 -c $< -o $@

clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: clean install

