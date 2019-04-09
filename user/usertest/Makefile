OUTFILE = usertest
OBJS = usertest.o

TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(TOP_DIR)/root/bin

CFLAGS = -ggdb -nostdinc -fno-pic -O2
LDFLAGS = -nostdlib -Wl,-z,max-page-size=4096,-T$(TOP_DIR)/user/user.ld

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

install:
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY: clean install
