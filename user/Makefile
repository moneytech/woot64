SUBDIRS = \
	libsyscalls \
	libc \
	libwoot \
	init \
	usertest

all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

install:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

.PHONY: all clean install
