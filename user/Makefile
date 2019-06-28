SUBDIRS = \
	libsyscalls \
	libc \
	libz \
	libpng \
	libfreetype \
	libwoot \
	libgmp \
	libmpfr \
	console \
	filemanager \
	init \
	inputhandler \
	ted \
	timekeeper \
	usertest \
	windowmanager \
	yasm \
	icons \
	fonts

all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

install:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

.PHONY: all clean install
