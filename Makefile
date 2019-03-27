MOUNTPOINT = /mnt
LOOP_DEVICE = /dev/loop7
QEMU = /usr/bin/qemu-system-x86_64

SUBDIRS = kernel

# tools used
CC = clang
CXX = clang++
ASM = yasm
LD = ld
AR = ar

export CC CXX ASM LD AR

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

install:
	

hdd.img: hdd-empty-ext2.img.gz
	gunzip -c $? > $@
	$(MAKE) try-mount
	grub-install --boot-directory=$(MOUNTPOINT)/boot $(LOOP_DEVICE)
	$(MAKE) try-umount

try-mount:
	sudo losetup -P $(LOOP_DEVICE) hdd.img
	sudo mount $(LOOP_DEVICE)p1 $(MOUNTPOINT)

try-umount:
	-sudo umount $(MOUNTPOINT)
	-sudo losetup -D $(LOOP_DEVICE)

clean:

run:
	$(QEMU) -s -m 256 -M q35 -drive type=raw,file=hdd.img

.PHONY: clean run

