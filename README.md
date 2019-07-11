WOOT is yet another hobby operating system. It's 64 bit version of previous projects which can be found [here](https://github.com/pvc988/woot) and [here](https://github.com/pvc988/woot-os).

Makefiles are meant to be built under Linux. Use `make hdd.img` to create hard disk image file. `make` will build all needed binaries. `make install` will install all needed files to hard disk image. Image file can then be run in a VM or dd'd to hard disk and booted on real PC.

Most of the project uses clang as its default compiler but should also compile just fine with GCC.


Most important changes in this version (0.4):

* 64 bit support (x86-64 and AMD64)
* usermode window manager
* a lot of UI improvements
* inter-process communication
* simple RPC mechanism
* unix-like pipes

And this is how it looks for now:
![Screenshot](screenshot.png?raw=true "Screenshot")

