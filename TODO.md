# TODO

## triumph
- Add test suite for all triumph instructions using the input/output from the console.
- Add debugging functionality (in the console), like a simple version of gdb, to make debugging programs much easier. It should allow reading and setting registers, jumping to new addresses, reading and tweaking the stack and memory, and allow restart of normal CPU flow.
- Add a clock, maybe expose as another peripheral via a memory bank. Get the Screen, CPU, and console/disk I/O threads all running in sync with the clock.
- Add real-time keyboard input, through the screen (SDL has KEY_IN events and already pressing 'F' dumps the framebuffer to std::cout and 'T' dumps the tilemap)
- System resolution - currently fixed at 486x486 (486 = 54 tiles * 9 pixels per tile). Look into SDL2 options to allow resizing and automatic rescaling of screen.
- Use CLI11 for triumph as well for command line options. Some possibilties:
   * Option forrunning a .tri file directly from $DMM, rather than loading it from a disk
   * Run in console-only mode, no screen (maybe even compile in console-only mode?)

## tas
- Fix a bug with chars - currently '\n' and other control characters will not be parsed as CHAR tokens.
- When debugger is working, add "BREAK" instruction so that debugging mode can be activated

## General
- Write test programs! Some ideas:
  * Print Fibonacci numbers (up to 9841, or even beyond...) onto screen
  * Simulate Conway's game of life
  * Brainfuck compiler (with input from stdin, output onto stdout)
  * Maybe a little too ambitious: games - Pong, Chess, 'Zelda' (Triumph forks? ;))
