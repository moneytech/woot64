OUTFILE = libsyscalls.so
OBJS = syscalls.o

TOP_DIR ?= $(shell realpath ../..)
LIB_DIR ?= $(TOP_DIR)/root/lib

CC = $(TOP_DIR)/user/cc

LDFLAGS = -Ttext=0x0000000040000000 -shared -Wl,-soname=$(OUTFILE)

export TOP_DIR

all: $(OUTFILE)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

install:
	mkdir -p $(LIB_DIR)
	cp $(OUTFILE) $(LIB_DIR)/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY: clean install
