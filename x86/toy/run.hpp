//
//  run.hpp
//  x86
//
//  Created by Chidume Nnamdi on 06/04/2026.
//

#ifndef run_hpp
#define run_hpp

#include <stdio.h>

#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

using namespace std;

/* ===================== FLAGS ===================== */
struct Flags {
    bool Z = false; // Zero
    bool C = false; // Carry
    bool N = false; // Negative
    bool O = false; // Overflow
};

/* ===================== ALU ===================== */
class ALU {
public:
    static uint8_t add(uint8_t a, uint8_t b, Flags &f) {
        uint16_t res = a + b;
        f.C = res > 0xFF;
        f.Z = (uint8_t)res == 0;
        f.N = (res & 0x80);
        f.O = ((a ^ res) & (b ^ res) & 0x80);
        return (uint8_t)res;
    }

    static uint8_t sub(uint8_t a, uint8_t b, Flags &f) {
        int16_t res = a - b;
        f.C = a >= b;
        f.Z = (uint8_t)res == 0;
        f.N = (res & 0x80);
        f.O = ((a ^ b) & (a ^ res) & 0x80);
        return (uint8_t)res;
    }

    static uint8_t _and(uint8_t a, uint8_t b, Flags &f) {
        uint8_t res = a & b;
        f.Z = res == 0;
        return res;
    }

    static uint8_t _or(uint8_t a, uint8_t b, Flags &f) {
        uint8_t res = a | b;
        f.Z = res == 0;
        return res;
    }
};

/* ===================== MEMORY ===================== */
class RAM {
public:
    vector<uint8_t> mem;

    RAM(size_t size = 256) {
        mem.resize(size, 0);
    }

    uint8_t read(uint8_t addr) {
        return mem[addr];
    }

    void write(uint8_t addr, uint8_t val) {
        mem[addr] = val;
    }
};

/* ===================== INSTRUCTION ===================== */
struct Instruction {
    uint8_t opcode;
    uint8_t r1;
    uint8_t r2;
    uint8_t imm;
};

/* ===================== PIC =====================*/
struct PIC {
    queue<uint8_t> irqs;

    void raise(uint8_t irq) {
        irqs.push(irq);
    }

    int next() {
        if (irqs.empty()) return -1;
        int v = irqs.front();
        irqs.pop();
        return v;
    }
};

struct Process {
    uint8_t R[4];
    uint8_t ACC;
    uint8_t PC;
    uint8_t SP;
};

/* ===================== CPU ===================== */
class CPU {
public:
    uint8_t SP = 0xFF;              // Stack Pointer
    uint8_t IDT[256] = {0};        // Interrupt Vector Table
    bool interruptEnabled = true;

    int cycles = 0;                // clock cycles

    // Forward declare PIC (we'll define it soon)
    struct PIC* pic = nullptr;
    vector<Process> processes;
    int currentProcess = 0;
    
    uint8_t R[4] = {0};   // General registers
    uint8_t ACC = 0;      // Accumulator
    uint8_t PC = 0;       // Program Counter
    uint32_t IR = 0;      // Instruction Register

    Flags flags;
    RAM ram;

    bool running = true;

    CPU() : ram(256) {}

    /* -------- FETCH -------- */
    void fetch() {
        IR = 0;
        IR |= ram.read(PC++) << 24;
        IR |= ram.read(PC++) << 16;
        IR |= ram.read(PC++) << 8;
        IR |= ram.read(PC++);
    }

    /* -------- DECODE -------- */
    Instruction decode() {
        Instruction inst;
        inst.opcode = (IR >> 24) & 0xFF;
        inst.r1     = (IR >> 16) & 0xFF;
        inst.r2     = (IR >> 8) & 0xFF;
        inst.imm    = IR & 0xFF;
        return inst;
    }

    /* -------- EXECUTE -------- */
    void execute(const Instruction &i) {
        switch (i.opcode) {
            case 0x01: // LOAD R, IMM
                R[i.r1] = i.imm;
                break;

            case 0x02: // ADD R1, R2
                R[i.r1] = ALU::add(R[i.r1], R[i.r2], flags);
                break;

            case 0x03: // SUB R1, R2
                R[i.r1] = ALU::sub(R[i.r1], R[i.r2], flags);
                break;

            case 0x04: // AND
                R[i.r1] = ALU::_and(R[i.r1], R[i.r2], flags);
                break;

            case 0x05: // OR
                R[i.r1] = ALU::_or(R[i.r1], R[i.r2], flags);
                break;

            case 0x06: // LOADM R, [addr]
                R[i.r1] = ram.read(i.imm);
                break;

            case 0x07: // STOREM R, [addr]
                ram.write(i.imm, R[i.r1]);
                break;

            case 0x08: // JMP addr
                PC = i.imm;
                break;

            case 0x09: // JZ addr
                if (flags.Z)
                    PC = i.imm;
                break;

            case 0x0A: // MOV ACC, R
                ACC = R[i.r1];
                break;

            case 0x0B: // MOV R, ACC
                R[i.r1] = ACC;
                break;

            case 0xFF: // HALT
                running = false;
                break;

            default:
                cout << "Unknown opcode\n";
                running = false;
        }
    }

    /* -------- RUN -------- */
    void run() {
        while (running) {
            
            tick(); // clock
            
            // check interrupts
            int irq = pic->next();
            if (irq != -1) {
                triggerInterrupt(irq);
            }
            
            fetch();
            Instruction inst = decode();
            execute(inst);
            debug();
        }
    }

    /* -------- DEBUG -------- */
    void debug() {
        cout << "PC: " << (int)PC << " | ";
        for (int i = 0; i < 4; i++)
            cout << "R" << i << ":" << (int)R[i] << " ";
        cout << "ACC:" << (int)ACC;
        cout << " | Z:" << flags.Z << " C:" << flags.C
             << " N:" << flags.N << " O:" << flags.O << endl;
    }
    
    void triggerInterrupt(uint8_t intNo) {
        if (!interruptEnabled) return;

        // push PC to stack
        ram.write(SP--, PC);

        // jump to handler
        PC = IDT[intNo];
    }
    
    void tick() {
        cycles++;

        // fire timer interrupt every 5 instructions
        if (cycles % 5 == 0) {
            pic->raise(0x20); // timer IRQ
        }
    }
    
    void saveProcess() {
        auto &p = processes[currentProcess];
        for (int i = 0; i < 4; i++) p.R[i] = R[i];
        p.ACC = ACC;
        p.PC = PC;
        p.SP = SP;
    }

    void loadProcess() {
        auto &p = processes[currentProcess];
        for (int i = 0; i < 4; i++) R[i] = p.R[i];
        ACC = p.ACC;
        PC = p.PC;
        SP = p.SP;
    }

    void scheduleNext() {
        saveProcess();

        currentProcess = (currentProcess + 1) % processes.size();

        loadProcess();
    }
};

/* ===================== PROGRAM LOADER ===================== */
void loadProgram(CPU &cpu, vector<uint32_t> program) {
    int addr = 0;
    for (auto instr : program) {
        cpu.ram.write(addr++, (instr >> 24) & 0xFF);
        cpu.ram.write(addr++, (instr >> 16) & 0xFF);
        cpu.ram.write(addr++, (instr >> 8) & 0xFF);
        cpu.ram.write(addr++, instr & 0xFF);
    }
}

/* ===================== MAIN ===================== */
int main() {
    CPU cpu;

    vector<uint32_t> program = {
        0x0100000A, // LOAD R0, 10
        0x01010014, // LOAD R1, 20
        0x02000100, // ADD R0, R1
        0x07000010, // STORE R0 -> [16]
        0x06020010, // LOADM R2, [16]
        0x03020100, // SUB R2, R1
        0x09000020, // JZ -> addr 32
        0x08000008, // JMP -> loop
        0xFF000000  // HALT
    };

    loadProgram(cpu, program);
    cpu.run();

    return 0;
}

#endif /* run_hpp */
