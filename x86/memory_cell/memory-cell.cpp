//
//  memory-cell.cpp
//  x86
//
//  Created by Chidume Nnamdi on 22/03/2026.
//

#include "memory-cell.hpp"

int memory_cell(int s, int i) {
    
    int b, d;
    
    int a = nand_gate(i, s);
    int c = nand_gate(a, s);
    d = nand_gate(b, c);
    b = nand_gate(a, d);

    return b;
    
}
