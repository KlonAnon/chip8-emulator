#include "chip8.hpp"
#include <fstream>
#include <iostream>

void Chip8::reset()
{
    I = 0;
    pc = 0x200;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;

    for (int i = 0; i < 4096; i++)
        memory[i] = 0;
    for (int i = 0; i < 16; i++)
    {
        V[i] = 0;
        stack[i] = 0;
        keys[i] = false;
    }
    for (int i = 0; i < 64 * 32; i++)
        display[i] = false;
}

bool Chip8::loadRom(const char *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cout << "Error opening file: " << filename << "\n";
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    const uint16_t startAdress = 0x200;
    if (size + startAdress > 4096)
    {
        std::cout << "File to big for chip8 memory\n";
        return false;
    }

    file.read(reinterpret_cast<char *>(&memory[startAdress]), size);
    if (!file)
    {
        std::cout << "Error reading File\n";
        return false;
    }

    std::cout << "ROM-File opened successfully: " << filename << " (" << size << " Bytes)\n";
    return true;
}

void Chip8::cycle()
{
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;

    uint16_t nnn = opcode & 0x0FFF;
    uint16_t kk = opcode & 0x00FF;

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode)
        {
        case 0x00E0:
            for (int i = 0; i < 64 * 32; i++)
                display[i] = false;
            break;
        case 0x00EE:
            pc = stack[sp];
            sp -= 1;
            break;
        }
        break;
    case 0x1000:
        pc = nnn;
        break;
    case 0x2000:
        sp += 1;
        stack[sp] = pc;
        pc = nnn;
        break;
    case 0x3000:
        if (V[x] == kk)
            pc += 4;
        break;
    case 0x4000:
        if (V[x] != kk)
            pc += 4;
        break;
    case 0x5000:
        if (V[x] == V[y])
            pc += 4;
        break;
    case 0x6000:
        V[x] = kk;
        break;
    case 0x7000:
        V[x] += kk;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0:
            V[x] = V[y];
            break;
        case 0x1:
            V[x] |= V[y];
            break;
        case 0x2:
            V[x] &= V[y];
            break;
        case 0x3:
            V[x] ^= V[y];
            break;
        case 0x4:
            V[0xF] = (V[y] > 0xFF - V[x]) ? 1 : 0;
            V[x] += V[y];
            break;
        case 0x5:
            V[0xF] = (V[x] > V[y]) ? 1 : 0;
            V[x] -= V[y];
            break;
        case 0x6:
            V[0xF] = V[x] & 0x1;
            V[x] >>= 1;
        case 0x7:
            V[0xF] = (V[y] > V[x]) ? 1 : 0;
            V[x] = V[y] - V[x];
            break;
        case 0xE:
            V[0xF] = V[x] & 0x80;
            V[x] <<= 1;
            break;
        }
        break;
    case 0x9000:
        if (V[x] != V[y])
            pc += 4;
        break;
    case 0xA000:
        I = nnn;
        break;
    case 0xB000:
        pc = nnn + V[0];
        break;
    case 0xC000:
        uint8_t r = static_cast<uint8_t>(std::rand() % 256);
    }
}