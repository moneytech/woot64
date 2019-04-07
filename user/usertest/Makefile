OUTFILE = usertest
OBJS = usertest.o

TOP_DIR ?= $(shell realpath ../..)
BIN_DIR ?= $(TOP_DIR)/root/bin

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUTFILE)

install:
	mkdir -p $(BIN_DIR)
	cp $(OUTFILE) $(BIN_DIR)/$(OUTFILE)

clean:
	rm -f $(OBJS) $(OUTFILE)

.PHONY: clean install
