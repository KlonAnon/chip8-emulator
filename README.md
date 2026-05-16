# Chip8 Emulator

A small Chip8 emulator I built to get more familiar with C++ and practical emulation.

As a fast lookup reference for the Chip8 technicalities I used:
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

## Build (Windows, MSYS2 MINGW64)

Run in the mingw64 shell:

```bash
g++ main.cpp chip8.cpp -o chip8.exe $(pkg-config --cflags --libs sdl2 SDL2_mixer)
```
