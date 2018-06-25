# chip8-emulator
A [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) emulator.
This was written partly as an exercise in writing emulators, mostly for fun. It currently does not support sound, 
though i might add this at a later date. It also includes a debug screen, which can be used to test ROMs.

Although the Chip-8 originially doesn't specify any execution speed, it's currently limited to 1000 hz, which seems to work alright with most ROMs.

### Dependencies
- SDL 2.0
- ncurses (for the debug screen)

### How to run
Compile with `make all`. Alternatively `make debug` to compile with the debug screen
```
usage: c8emu input_program
```
