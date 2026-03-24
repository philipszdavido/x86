//
//  mod.cpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#include "mod.hpp"

Mod decodeModRM(CPU cpu) {
    int byte = read8(cpu.eip++);
    int mod = (byte >> 6) & 0b11;
    int reg = (byte >> 3) & 0b111;
    int rm  = byte & 0b111;
    
    int addr = 0;
    
    if (mod == 0) {
        if (rm == 5) {
            addr = read32(cpu.eip);
            cpu.eip += 4;
        } else {
            addr = cpu.regs[rm];
        }
    } else if (mod == 1) {
        int disp8 = (read8(cpu.eip++) << 24) >> 24; // sign extend
        addr = cpu.regs[rm] + disp8;
    } else if (mod == 2) {
        int disp32 = read32(cpu.eip);
        cpu.eip += 4;
        addr = cpu.regs[rm] + disp32;
    }
    
    return { mod, reg, rm, addr };
}
