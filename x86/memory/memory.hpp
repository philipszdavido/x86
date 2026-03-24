//
//  memory.hpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#ifndef memory_hpp
#define memory_hpp

#include <stdio.h>
#include <iostream>

int read8(int addr);
int read32(int addr);
void write32(int addr, int value);


#endif /* memory_hpp */
