#include "chip8.hpp"
#include <iostream>
#include <ctime>
#include <SDL2/SDL.h>

const int SCALE = 10;
const int WIDTH = 64 * SCALE;
const int HEIGHT = 32 * SCALE;

int main(int argc, char *argv[])
{
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  Chip8 chip;
  chip.reset();
  std::cout << "Chip8 Emulator Start\n";
  chip.loadRom("games/Pong.ch8");

  SDL_Window *window = SDL_CreateWindow(
      "Chip-8 Emulator",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WIDTH,
      HEIGHT,
      0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool running = true;
  SDL_Event event;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
          running = false;
        }
        mapKey(chip, event.key.keysym.sym, true);
      }
      if (event.type == SDL_KEYUP)
        mapKey(chip, event.key.keysym.sym, false);
    }

    chip.cycle();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < 32; y++)
    {
      for (int x = 0; x < 64; x++)
      {
        int index = y * 64 + x;

        if (chip.display[index])
        {
          SDL_Rect rect;
          rect.x = x * SCALE;
          rect.y = y * SCALE;
          rect.w = SCALE;
          rect.h = SCALE;

          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void mapKey(Chip8 chip, SDL_Keycode keycode, bool keydown)
{
  switch (keycode)
  {
  case SDLK_1:
    chip.keys[0x1] = keydown;
    break;
  case SDLK_2:
    chip.keys[0x2] = keydown;
    break;
  case SDLK_3:
    chip.keys[0x3] = keydown;
    break;
  case SDLK_4:
    chip.keys[0xC] = keydown;
    break;

  case SDLK_q:
    chip.keys[0x4] = keydown;
    break;
  case SDLK_w:
    chip.keys[0x5] = keydown;
    break;
  case SDLK_e:
    chip.keys[0x6] = keydown;
    break;
  case SDLK_r:
    chip.keys[0xD] = keydown;
    break;

  case SDLK_a:
    chip.keys[0x7] = keydown;
    break;
  case SDLK_s:
    chip.keys[0x8] = keydown;
    break;
  case SDLK_d:
    chip.keys[0x9] = keydown;
    break;
  case SDLK_f:
    chip.keys[0xE] = keydown;
    break;

  case SDLK_z:
    chip.keys[0xA] = keydown;
    break;
  case SDLK_x:
    chip.keys[0x0] = keydown;
    break;
  case SDLK_c:
    chip.keys[0xB] = keydown;
    break;
  case SDLK_v:
    chip.keys[0xF] = keydown;
    break;
  }
}