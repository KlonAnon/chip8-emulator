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

    for (int i = 0; i < 80; i++)
    {
        memory[FONT_START + i] = FONTSET[i];
    }
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
            break;
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
    {
        uint8_t r = static_cast<uint8_t>(std::rand() % 256);
        V[x] = r & kk;
        break;
    }
    case 0xD000:
    {
        uint8_t n = opcode & 0x000F;
        for (int row = 0; row < n; row++)
        {
            uint8_t spriteByte = memory[I + row];
            for (int bit = 0; bit < 8; bit++)
            {
                uint8_t mask = 0x80 >> bit;

                if (spriteByte & mask)
                {
                    int xPos = (V[x] + bit) % 64;
                    int yPos = (V[y] + row) % 32;
                    int index = yPos * 64 + xPos;

                    if (display[index])
                        V[0xF] = 1;

                    display[index] = !display[index];
                }
            }
        }
        break;
    }
    case 0xE000:
        switch (kk)
        {
        case 0x9E:
            if (keys[V[x]])
                pc += 4;
            break;
        case 0xA1:
            if (!keys[V[x]])
                pc += 4;
            break;
        }
        break;
    case 0xF000:
        switch (kk)
        {
        case 0x07:
            V[x] = delayTimer;
            break;
        case 0x0A:
        {
            bool keyPressed = false;
            for (int k = 0; k < 16; k++)
            {
                if (keys[k])
                {
                    V[x] = k;
                    keyPressed = true;
                    break;
                }
            }
            if (!keyPressed)
                pc -= 2;
            break;
        }
        case 0x15:
            delayTimer = V[x];
            break;
        case 0x18:
            soundTimer = V[x];
            break;
        case 0x1E:
            I += V[x];
            break;
        case 0x29:
        {
            uint8_t digit = V[x] & 0x0F;
            I = FONT_START + digit * 5;
            break;
        }
        case 0x33:
            memory[I] = V[x] / 100;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I + 2] = V[x] % 10;
            break;
        case 0x55:
            for (int i = 0; i <= x; i++)
            {
                memory[I + i] = V[i];
            }
            break;
        case 0x65:
            for (int i = 0; i <= x; i++)
            {
                V[i] = memory[I + i];
            }
            break;
        }
    }
}