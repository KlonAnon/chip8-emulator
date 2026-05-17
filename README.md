# Chip8 Emulator

Small Chip-8 emulator built to get more familiar with C++ and practical emulation.

## References

- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- https://chip8.gulrak.net/

## Tests

- https://github.com/Timendus/chip8-test-suite

## Build (Windows, MSYS2 MINGW64)

Run in the MINGW64 shell:

```bash
g++ main.cpp chip8.cpp -o chip8.exe $(pkg-config --cflags --libs sdl2 SDL2_mixer)
```
