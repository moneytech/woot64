MOUNTPOINT = /mnt
LOOP_DEVICE = /dev/loop7
QEMU = /usr/bin/qemu-system-x86_64
IMGFILE = hdd.img

EXTRA_FILES = \
	root/logo.png \
	root/clock_small.png \
	root/uisymbols.ttf

SUBDIRS = kernel user

# tools used
CC = clang
CXX = clang++
ASM = yasm
LD = ld
AR = ar

WOOT_TOP_DIR = $(shell realpath .)

export CC CXX ASM LD AR WOOT_TOP_DIR

all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir; done

install: $(IMGFILE)
	mkdir -p ./root
	$(MAKE) $(EXTRA_FILES)
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install; done
	$(MAKE) try-mount
	-cp -r ./root/* $(MOUNTPOINT)
	$(MAKE) try-umount

$(IMGFILE): hdd-empty-ext2.img.gz
	gunzip -c $? > $@
	$(MAKE) try-mount
	grub-install --boot-directory=$(MOUNTPOINT)/boot $(LOOP_DEVICE)
	$(MAKE) try-umount

root/logo.png: logo.png
	cp $? $@

root/clock_small.png: clock_small.png
	cp $? $@

root/uisymbols.ttf: uisymbols.ttf
	cp $? $@

try-mount:
	sudo losetup -P $(LOOP_DEVICE) hdd.img
	sudo mount $(LOOP_DEVICE)p1 $(MOUNTPOINT)

try-umount:
	-sudo umount $(MOUNTPOINT)
	-sudo losetup -D $(LOOP_DEVICE)

clean:
	-for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

distclean: clean clean-$(IMGFILE)
	rm -rf ./root
	
clean-$(IMGFILE):
	rm -f $(IMGFILE)

QEMU_ARGS = -s -m 256 -M q35 -drive format=raw,file=hdd.img -debugcon vc

run:
	$(QEMU) $(QEMU_ARGS)

run-dint:
	$(QEMU) $(QEMU_ARGS) -d int

run-kvm:
	$(QEMU) $(QEMU_ARGS)

.PHONY: clean distclean clean-$(IMGFILE) run run-qemu install

