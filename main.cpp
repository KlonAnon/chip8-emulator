#include <iostream>
#include <fstream>
#include <cstdint>

struct Chip8 {
    uint8_t  memory[4096] = { 0 };
    uint8_t  V[16]        = { 0 };
    uint16_t I            = 0;
    uint16_t pc           = 0x200;
    uint16_t stack[16]    = { 0 };
    uint8_t  sp           = 0;
    uint8_t  delayTimer   = 0;
    uint8_t  soundTimer   = 0;
    bool     display[64 * 32] = { false };
    bool     keys[16]         = { false };

    bool loadRom(const char* filename);
    void cycle();
    void reset() {
        I = 0;
        pc = 0x200;
        sp = 0;
        delayTimer = 0;
        soundTimer = 0;

        for (int i = 0; i < 4096; i++) memory[i] = 0;
        for (int i = 0; i < 16; i++) {
            V[i] = 0;
            stack[i] = 0;
            keys[i] = false;
        }
        for (int i = 0; i < 64 * 32; i++) display[i] = false;
    }
};

bool Chip8::loadRom(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << "\n";
        return false;
    } 
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    const uint16_t startAdress = 0x200;
    if (size + startAdress > 4096) {
      std::cout << "File to big for chip8 memory\n";
      return false;
    }

    file.read(reinterpret_cast<char*>(&memory[startAdress]), size);
    if (!file) {
      std::cout << "Error reading File\n";
      return false;
    }

    std::cout << "ROM-File opened successfully: " << filename << " (" << size << " Bytes)\n";
    return true;
}

void Chip8::cycle() {
  uint16_t opcode = (memory[pc] << 8 | memory[pc + 1]);
  pc += 2;

  std::cout << "PC: 0x" << std::hex << pc - 2 << "  OPCODE: 0x" << opcode << "\n";
}

int main() {
    Chip8 chip;
    std::cout << "Chip8 Emulator Start\n";
    chip.loadRom("games/Tetris.ch8");
    for (int i = 0; i < 5; i++) {
      chip.cycle();
    }
    return 0;
}