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

CC = $(WOOT_TOP_DIR)/user/cc

CFLAGS += -fPIC -Wno-undefined-internal
LDFLAGS = -shared -Wl,-soname=$(SHARED_LIB)
LIBS = -Wl,--start-group,-lc,-lsyscalls,-lpng,-lfreetype,--end-group

export WOOT_TOP_DIR

all: $(SHARED_LIB) $(STATIC_LIB)

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(SHARED_LIB) $(LIBS)

install: $(LIB_DIR)/$(SHARED_LIB) $(CFG_DIR)/editors.cfg

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

$(CFG_DIR)/editors.cfg: editors.cfg
	mkdir -p $(CFG_DIR)
	cp $< $@

pixmap.o: pixmap.c
	$(CC) $(CFLAGS) -O2 -c $< -o $@

font.o: font.c
	$(CC) $(CFLAGS) -O2 -c $< -o $@

clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: clean install

