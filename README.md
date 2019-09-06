NOTICE
------

This version is about at the end of its life. I am now working on version 0.5 called WOOTMP. MP meaning (hopefully) both MultiProcessor and MultiPlatform. Apart from that, the rest of the OS is going to stay the same. It's going to get available on GitHub soon.

Original README
---------------

WOOT is yet another hobby operating system. It's 64 bit version of previous projects which can be found [here](https://github.com/pvc988/woot) and [here](https://github.com/pvc988/woot-os).

Makefiles are meant to be built under Linux. First use `bootstrap` script to build the cross compiler. Then you can use `make hdd.img` to create hard disk image file. `make` will build all needed binaries. `make install` will install all needed files to hard disk image. Image file can then be run in a VM or dd'd to hard disk and booted on real PC.

Some parts of the project use clang. As for now the changeover to GCC is in progress so proper compilation from scatch may be difficult.


Most important changes in this version (0.4):

* 64 bit support (x86-64 and AMD64)
* usermode window manager
* a lot of UI improvements
* inter-process communication
* simple RPC mechanism
* unix-like pipes
* UEFI and GPT support

And this is how it looks for now:
![Screenshot](screenshot.png?raw=true "Screenshot")

