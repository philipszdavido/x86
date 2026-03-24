//
//  sys.hpp
//  x86
//
//  Created by Chidume Nnamdi on 20/03/2026.
//

#ifndef sys_hpp
#define sys_hpp

#include <stdio.h>
#include <iostream>
#include "./../cpu/cpu.hpp"
#include "./../memory/memory.hpp"

using namespace std;

void handleSyscall(CPU cpu);

#endif /* sys_hpp */
