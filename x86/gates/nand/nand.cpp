//
//  nand.cpp
//  x86
//
//  Created by Chidume Nnamdi on 22/03/2026.
//

#include "nand.hpp"

int nand_gate(int i0, int i1) {
    return !(i0 && i1);
}
