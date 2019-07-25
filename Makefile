MOUNTPOINT = mnt
QEMU = /usr/bin/qemu-system-x86_64
QEMU_ARGS = -s -m 256 -M q35 -debugcon vc -net none
OVMF_ARGS = -drive if=pflash,format=raw,unit=0,file=OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=OVMF_VARS.fd
IMGFILE = hdd.img
IMGFILE_EFI = hdd-efi.img

EXTRA_FILES = \
	root/logo.png \
	root/clock_small.png

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

install-efi: $(IMGFILE_EFI)
	mkdir -p ./root
	$(MAKE) $(EXTRA_FILES)
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir install; done
	$(MAKE) try-mount-efi
	-cp -r ./root/* $(MOUNTPOINT)
	$(MAKE) try-umount

$(IMGFILE): hdd-grub-ext2.img.gz
	gunzip -c $? > $@

$(IMGFILE_EFI): hdd-grub-ext2-efi.img.gz
	gunzip -c $? > $@

root/logo.png: logo.png
	cp $? $@

root/clock_small.png: clock_small.png
	cp $? $@

try-mount:
	mkdir -p $(MOUNTPOINT)
	guestmount -a $(IMGFILE) -m /dev/sda1 $(MOUNTPOINT)

try-mount-efi:
	mkdir -p $(MOUNTPOINT)
	guestmount -a $(IMGFILE_EFI) -m /dev/sda2 $(MOUNTPOINT)

try-umount:
	guestunmount mnt
	sleep 1

clean:
	-for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

distclean: clean clean-$(IMGFILE)
	rm -rf ./root
	
clean-$(IMGFILE):
	rm -f $(IMGFILE)

clean-$(IMGFILE_EFI):
	rm -f $(IMGFILE_EFI)

run:
	$(QEMU) $(QEMU_ARGS) -drive format=raw,file=$(IMGFILE)

run-efi:
	$(QEMU) $(QEMU_ARGS) $(OVMF_ARGS) -drive format=raw,file=$(IMGFILE_EFI)

run-dint:
	$(QEMU) $(QEMU_ARGS) -d int -drive format=raw,file=$(IMGFILE)

run-efi-dint:
	$(QEMU) $(QEMU_ARGS) -d int $(OVMF_ARGS) -drive format=raw,file=$(IMGFILE_EFI)

run-kvm:
	$(QEMU) $(QEMU_ARGS) -enable-kvm -drive format=raw,file=$(IMGFILE)

run-efi-kvm:
	$(QEMU) $(QEMU_ARGS) -enable-kvm $(OVMF_ARGS) -drive format=raw,file=$(IMGFILE_EFI)

.PHONY: clean distclean clean-$(IMGFILE) clean-$(IMGFILE_EFI) run run-efi run-dint run-efi-dint run-kvm run-efi-kvm install install-efi try-mount try-mount-efi try-umount

