#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "../memory/memory.h"

class CPU {
public:
    CPU();
    ~CPU();

    void reset();
    void executeCycle();

    void handleInterrupts();
    void setFlag();
    bool checkFlag();

private:
    // CPU registers
    uint8_t A, F;
    uint8_t B, C, D, E, H, L;
    uint16_t SP;
    uint16_t PC;

    // Memory management component
    Memory memory;

    // CPU Flags (could be part of F register)
    bool f_Z;
    bool f_N;
    bool f_H;
    bool f_C;

    // Private methods for CPU functionality
    void executeInstruction(uint8_t opcode);
    void add(uint8_t value);
    void sub(uint8_t value);

    // Opcodes handling methods
    void opcode00();
    void opcode01();
};

#endif // CPU_H
