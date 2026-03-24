//
//  main.cpp
//  x86
//
//  Created by Chidume Nnamdi on 19/03/2026.
//

#include <iostream>
#include "cpu/cpu.hpp"
#include "memory/memory.hpp"
#include "mod/mod.hpp"

#include "memory_cell/memory-cell.hpp"
#include "gates/nand/nand.hpp"
#include "gates/and/and.hpp"

using namespace std;

//void _run(CPU cpu) {
//  while (true) {
//    int opcode = memory[cpu.eip++];
//    
//    switch (opcode) {
//      case 0xB8: // MOV EAX, imm32
//        cpu.eax = read32(cpu.eip);
//        cpu.eip += 4;
//        break;
//
//      case 0x05: // ADD EAX, imm32
//        cpu.eax += read32(cpu.eip);
//        cpu.eip += 4;
//        break;
//
//      case 0xE9: // JMP rel32
//        int offset = read32(cpu.eip);
//        cpu.eip += offset + 4;
//        break;
//
//      default:
//            throw &"Unknown opcode: " [ opcode];
//    }
//  }
//}

void run(CPU cpu) {
    while (true) {
        int opcode = read8(cpu.eip++);
        
        switch (opcode) {
                
                // MOV r32, imm32
            case 0xB8: case 0xB9: case 0xBA: case 0xBB:
            case 0xBC: case 0xBD: case 0xBE: case 0xBF: {
                int reg = opcode - 0xB8;
                cpu.regs[reg] = read32(cpu.eip);
                cpu.eip += 4;
                break;
            }
                
                // MOV r/m32, r32
            case 0x89: {
                
                Mod m = decodeModRM(cpu);
                
                int mod = m.mod;
                int reg = m.reg;
                int rm = m.rm;
                int addr = m.addr;
                
                if (mod == 3) {
                    cpu.regs[rm] = cpu.regs[reg];
                } else {
                    write32(addr, cpu.regs[reg]);
                }
                break;
            }
                
                // MOV r32, r/m32
            case 0x8B: {
                
                Mod m = decodeModRM(cpu);
                
                int mod = m.mod;
                int reg = m.reg;
                int rm = m.rm;
                int addr = m.addr;
                
                if (mod == 3) {
                    cpu.regs[reg] = cpu.regs[rm];
                } else {
                    cpu.regs[reg] = read32(addr);
                }
                break;
            }
                
                // ADD r/m32, r32
            case 0x01: {
                
                Mod m = decodeModRM(cpu);
                
                int mod = m.mod;
                int reg = m.reg;
                int rm = m.rm;
                int addr = m.addr;
                
                if (mod == 3) {
                    cpu.regs[rm] += cpu.regs[reg];
                } else {
                    write32(addr, read32(addr) + cpu.regs[reg]);
                }
                break;
            }
                
                // CMP r/m32, r32
            case 0x39: {
                Mod m = decodeModRM(cpu);
                
                int mod = m.mod;
                int reg = m.reg;
                int rm = m.rm;
                int addr = m.addr;
                
                int a = mod == 3 ? cpu.regs[rm] : read32(addr);
                int b = cpu.regs[reg];
                int result = (a - b) >> 0;
                
                // set ZF
                if (result == 0) cpu.eflags |= (1 << 6);
                else cpu.eflags &= ~(1 << 6);
                
                break;
            }
                
                // JMP rel32
            case 0xE9: {
                int offset = read32(cpu.eip);
                cpu.eip += offset + 4;
                break;
            }
                
                // JZ rel32
            case 0x0F: {
                int next = read8(cpu.eip++);
                if (next == 0x84) {
                    int offset = read32(cpu.eip);
                    cpu.eip += 4;
                    if (cpu.eflags & (1 << 6)) {
                        cpu.eip += offset;
                    }
                }
                break;
            }
                
                // PUSH r32
            case 0x50: case 0x51: case 0x52: case 0x53:
            case 0x54: case 0x55: case 0x56: case 0x57: {
                int reg = opcode - 0x50;
                cpu.esp -= 4;
                write32(cpu.esp, cpu.regs[reg]);
                break;
            }
                
                // POP r32
            case 0x58: case 0x59: case 0x5A: case 0x5B:
            case 0x5C: case 0x5D: case 0x5E: case 0x5F: {
                int reg = opcode - 0x58;
                cpu.regs[reg] = read32(cpu.esp);
                cpu.esp += 4;
                break;
            }
                
            case 0xCD: { // INT imm8
              int intNum = read8(cpu.eip++);
              
              if (intNum == 0x80) {
                handleSyscall(cpu);
              }
              break;
            }
                
                // RET
            case 0xC3: {
                cpu.eip = read32(cpu.esp);
                cpu.esp += 4;
                break;
            }
                
            default:
                throw (&"Unknown opcode: " [ opcode]);
        }
    }
}

int main(int argc, const char * argv[]) {
    
//    const program = new Uint8Array(/* load program.bin */);
//    memory.set(program, 0x0000);
    
//    CPU cpu = CPU();
//    cpu.eip = 0x0000;
//    cpu.esp = 0x80000;

//    run(cpu);
    
//    cout << and_gate(0, 0) << endl;
//    cout << and_gate(1, 0) << endl;
//    cout << and_gate(1, 1) << endl;
    
    int set = 1;
    int input = 1;
    
    cout << memory_cell(set, input) << endl;
    set = 0;
    cout << memory_cell(set, input) << endl;
    
    input = 0;
    cout << memory_cell(set, input) << endl;
    cout << memory_cell(set, input) << endl;

//    cout << "EAX =" << cpu.eax;
    
    return EXIT_SUCCESS;
}
