OUTFILE = usertest
OUTFILE_STATIC = $(OUTFILE)_static
OBJS = usertest.o

TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(TOP_DIR)/root/bin

CC = $(TOP_DIR)/user/cc
LIBS = -lsyscalls

export TOP_DIR

all: $(OUTFILE)

static: $(OUTFILE_STATIC)

$(OUTFILE_STATIC): $(OBJS)
	$(CC) -static $(LDFLAGS) $? -o $@ $(LIBS)

$(OUTFILE): $(OBJS)
	$(CC) $(LDFLAGS) $? -o $@ $(LIBS)

install:
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE_STATIC) $(OUTFILE)

.PHONY: clean install static
