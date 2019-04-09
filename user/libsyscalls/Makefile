SHARED_LIB = libsyscalls.so
STATIC_LIB = libsyscalls.a
OBJS = syscalls.o

TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(TOP_DIR)/root/lib

CC = $(TOP_DIR)/user/cc

LDFLAGS = -shared -Wl,-Ttext=0x0000000040000000,-soname=$(SHARED_LIB)

export TOP_DIR

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(SHARED_LIB)

install:
	mkdir -p $(LIB_DIR)
	cp $(SHARED_LIB) $(LIB_DIR)/$(SHARED_LIB)

clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: clean install
