#include "chip8.hpp"
#include <iostream>

int main()
{
  Chip8 chip;
  std::cout << "Chip8 Emulator Start\n";
  chip.loadRom("games/Tetris.ch8");
  for (int i = 0; i < 5; i++)
  {
    chip.cycle();
  }
  return 0;
}