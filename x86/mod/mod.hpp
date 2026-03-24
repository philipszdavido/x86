//
//  mod.hpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#ifndef mod_hpp
#define mod_hpp

#include <stdio.h>
#include "../cpu/cpu.hpp"
#include "../memory/memory.hpp"

struct Mod {
    int mod;
    int reg;
    int rm;
    int addr;
};

Mod decodeModRM(CPU cpu);

#endif /* mod_hpp */
