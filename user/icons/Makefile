WOOT_TOP_DIR ?= $(shell realpath ../..)
ICONS_DIR = $(WOOT_TOP_DIR)/root/data/common/icons

ICONS = \
	$(ICONS_DIR)/directory.png \
	$(ICONS_DIR)/file.png \
	$(ICONS_DIR)/placeholder_24x24.png \
	$(ICONS_DIR)/program.png

$(ICONS_DIR)/directory.png: directory.png
$(ICONS_DIR)/file.png: file.png
$(ICONS_DIR)/placeholder_24x24.png: placeholder_24x24.png
$(ICONS_DIR)/program.png: program.png

$(ICONS_DIR)/%:
	mkdir -p $(ICONS_DIR) && cp -f $< $@

all:

install: $(ICONS)

uninstall:
	for file in $(ICONS); do rm -f $$file; done

clean:

.PHONY: clean install uninstall

