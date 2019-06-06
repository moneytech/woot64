WOOT is yet another hobby operating system. It's 64 bit version of previous projects which can be found [here](https://github.com/pvc988/woot) and [here](https://github.com/pvc988/woot-os).

Makefiles are meant to be built under Linux. Image creation makefile rules use sudo, so you may want to edit your sudoers file accordingly or deal with it your way. You'll most probably need losetup, mount and umount. Most of the project uses clang as its default compiler but should also compile just fine with GCC.

To build, just run make in top directory. With a little bit of luck everything will just build. hdd.img file containg ext2 disk image should be created. Image file can be run in a VM or dd'd to hard disk and booted on real PC.

Most important changes in this version (0.4):

* 64 bit support (x86-64 and AMD64)
* usermode window manager
* a lot of UI improvements
* inter-process communication
* simple RPC mechanism
* unix-like pipes

And this is how it looks for now:
![Screenshot](screenshot.png?raw=true "Screenshot")

