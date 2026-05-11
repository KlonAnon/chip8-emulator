#include "chip8.hpp"
#include <iostream>
#include <ctime>

int main()
{
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  Chip8 chip;
  std::cout << "Chip8 Emulator Start\n";
  chip.loadRom("games/Tetris.ch8");
  for (int i = 0; i < 5; i++)
  {
    chip.cycle();
  }
  return 0;
}