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
	thread.o \
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

CC = $(WOOT_TOP_DIR)/user/cc

CFLAGS += -fPIC -Wno-undefined-internal
LDFLAGS = -shared -Wl,-soname=$(SHARED_LIB)
LIBS = -Wl,--start-group,-lc,-lsyscalls,-lpng,-lfreetype,--end-group

export WOOT_TOP_DIR

all: $(SHARED_LIB)

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(SHARED_LIB) $(LIBS)

install: $(LIB_DIR)/$(SHARED_LIB)

$(LIB_DIR)/$(SHARED_LIB): $(SHARED_LIB)
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: clean install

