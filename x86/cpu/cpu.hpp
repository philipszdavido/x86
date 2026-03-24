//
//  cpu.hpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#ifndef cpu_hpp
#define cpu_hpp

#include <stdio.h>

class CPU {
    
public:
    int eax = 0;
    int ebx = 0;
    int ecx = 0;
    int edx = 0;
    
    int esi = 0;
    int edi = 0;
    int ebp = 0;
    int esp = 0;
    
    int eip = 0; // instruction pointer
    int eflags = 0;
    
    int regs[8] = { eax, ebx, ecx, edx, esi, edi, ebp, esp };
    
    int _eax() { return eax; }
    void _eax(int v) { eax = (v >> 0); }
    
    int _esp() { return esp; }
    void _esp(int v) { esp = v >> 0; }
    
};

#endif /* cpu_hpp */
