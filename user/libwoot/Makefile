SHARED_LIB = libwoot.so
STATIC_LIB = libwoot.a
OBJS = \
	input.o \
	misc.o \
	pixmap.o \
	process.o \
	rectangle.o \
	thread.o \
	video.o

TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(TOP_DIR)/root/lib

CC = $(TOP_DIR)/user/cc

CFLAGS += -fPIC -Wno-undefined-internal
LDFLAGS = -shared -Wl,-soname=$(SHARED_LIB)
LIBS = -lc -lsyscalls

export TOP_DIR

all: $(STATIC_LIB) $(SHARED_LIB)

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

