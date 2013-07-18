Lightweight crypto library designed primarily for embedded ARM architectures. It does not allocate memory, is written in pure C, and remains unbloated.

This library was written as part of the Bitcoin Titan project (a Bitcoin Hardware Wallet). Existing solutions were bloated and required memory allocation which was not preferred for an embedded system like the Titan project. Due to the library's small source code and simplicity, it is easy to audit and understand intimately.



NOTE: X86 support is experimental (X86 random module is not cryptographically strong yet; DO NOT USE).

#To Compile for STM32F4 target, Linux host#
* You will need [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded) installed.
* make
* Library will be in ./build/

#Unit Tests#
See: ./tests/README.md
