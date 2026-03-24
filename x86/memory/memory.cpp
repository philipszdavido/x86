//
//  memory.cpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#include "memory.hpp"

int* memory = new int(1024 * 1024); // 1MB RAM

int read8(int addr) {
  return memory[addr];
}

int read32(int addr) {
  return (
    memory[addr] |
    (memory[addr + 1] << 8) |
    (memory[addr + 2] << 16) |
    (memory[addr + 3] << 24)
  );
}

void write32(int addr, int value) {
  memory[addr] = value & 0xff;
  memory[addr + 1] = (value >> 8) & 0xff;
  memory[addr + 2] = (value >> 16) & 0xff;
  memory[addr + 3] = (value >> 24) & 0xff;
}
