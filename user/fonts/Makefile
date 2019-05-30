WOOT_TOP_DIR ?= $(shell realpath ../..)
FONTS_DIR = $(WOOT_TOP_DIR)/root/data/common/fonts

FONTS = \
	$(FONTS_DIR)/default.ttf \
	$(FONTS_DIR)/title.ttf \
	$(FONTS_DIR)/uisymbols.ttf

$(FONTS_DIR)/default.ttf: default.ttf
$(FONTS_DIR)/title.ttf: title.ttf
$(FONTS_DIR)/uisymbols.ttf: uisymbols.ttf

$(FONTS_DIR)/%:
	mkdir -p $(FONTS_DIR) && cp -f $< $@

all:

install: $(FONTS)

uninstall:
	for file in $(FONTS); do rm -f $$file; done

clean:

.PHONY: clean install uninstall

