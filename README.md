# triumph

TRIUMPH is an attempt at emulating a computer that runs on ternary logic - rather than the 0s and 1s of bits, this computer uses trits, which can have three states: +1, 0, or -1. Similarly to binary computers that use 8-bit bytes as the basic building block of computation, this computer uses _trytes_, which are composed of nine trits. This computer interprets trytes as machine instructions to work on arrays of Tryte memory, and performs mathematics using the _balanced ternary_ number system (see https://en.wikipedia.org/wiki/Balanced_ternary). See docs/spec.md for a more complete technical description of the principles behind ternary computing, and the opcodes this computer understands.

## Features

TRIUMPH has the following capabilities:

- 9-trit architecture; can address up to 19,683 (3^9) Trytes in its memory.
- 9 built-in general purpose registers, usable by all instructions, supplemented by PC (program counter), SP (stack pointer) and MB (memory bank) registers (implemented as special locations in memory). One register is a FLAG register, which is affected by various instructions.
- Ternary logic instructions have been implemented - ternary AND, ternary OR, and ternary STAR instructions. Another unique feature is the ternary jump instruction, which jumps to three possible destinations depending on the sign of the compare flag.
- Built in memory is 19,683 trytes, addressed by $MMM-$mmm (-9841 to 9841 in balanced septavingtesmal representation.)
- Lower third of memory $MMM-$Emm is bank switchable; different banks of memory may be accessed by setting a special MB (memory bank) register.
- TRIUMPH supports reading and writing to disks - text files containing only the characters A-M, 0 and a-m. These disks are lists of Trytes containing machine code or data. Each disk can have a maximum of 19,683 pages of 729 Trytes, and TRIUMPH can access up to 9,841 disks at once - inserting and removing disks is not supported at this time.
- TRIUMPH currently outputs graphics to a 54x54 tile display (for a resolution of 486x486 pixels.) This display is tile based; an array of 2,916 Trytes acts as a framebuffer, with each Tryte in this buffer pointing to one of 27 possible palettes (which can be adjusted freely at runtime) and one of 729 possible tiles in a 6,561 Tryte tilemap. TRIUMPH can render 27x27x27 RGB colours, although it can only display 81 on screen at one time.

## Repository layout

This repository has three main folders:

- /triumph, which contains the source code for the TRIUMPH ternary computer emulator and useful resource files (such as a "helloworld" disk.)
- /tas, where the assembler for TRIUMPH will reside. This program will convert ternary assembly language (as specified in docs/spec.md) into ternary machine code TRIUMPH can understand.
- /tools, which is a handy home for various little helpful tools, currently: a tilemap to Tryte array converter; a command-line tilemap viewer; and disk printing/examining utilities.

Additionally, /docs contains the specifications for the computer and opcodes, and /test will contain a test framework for verifying and benchmarking TRIUMPH's performance.

## Build

### Windows
TRIUMPH and tas are buildable in Visual Studio using its CMake extension. SDL2 libraries need to be installed and a "sdl2-config.cmake" file needs to be added to the library's install directory - this file is included in the root of this repository.

### MacOS and Linux
To build on MacOS and Linux, install CMake (using `brew install cmake` on MacOS and your favourite package manager on Linux) and the libraries and headers for SDL2. Put the header files and library files where your compiler/IDE can see them, and then build TRIUMPH by running the following in the top level of your working copy of this repository:

```
mkdir build && cd build
cmake ..
make
```

Then, run TRIUMPH by using `./triumph ../triumph/res/helloworld.td`. All being well, you should see the following:

![image](https://user-images.githubusercontent.com/38587102/170384923-6d26be3f-1af7-4f24-b7e7-f99acf52e8c0.png)


...and that's all it does right now! More demos to come soon...
