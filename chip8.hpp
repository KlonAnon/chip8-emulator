#pragma once
#include <cstdint>

struct Chip8
{
    uint8_t memory[4096] = {0};
    uint8_t V[16] = {0};
    uint16_t I = 0;
    uint16_t pc = 0x200;
    uint16_t stack[16] = {0};
    uint8_t sp = 0;
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;
    bool display[64 * 32] = {false};
    bool keys[16] = {false};

    void reset();
    bool loadRom(const char *filename);
    void cycle();
};