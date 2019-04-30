SUBDIRS = \
	libsyscalls \
	libc \
	libz \
	libpng \
	libfreetype \
	libwoot \
	libgmp \
	libmpfr \
	filemanager \
	init \
	inputhandler \
	usertest \
	windowmanager

all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

install:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

.PHONY: all clean install
