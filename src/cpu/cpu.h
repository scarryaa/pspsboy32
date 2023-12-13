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
    void updateTimers();
    uint8_t fetchInstruction();
    void executeExtendedInstruction(uint8_t opcode);

    // Stop flag
    static bool stopped;

    // Opcode function declarations
    uint8_t NoOperation();
    uint8_t Stop();
    uint8_t JR_NZ_i8();
    uint8_t JR_NC_i8();

    uint8_t LD_BC_d16();
    uint8_t LD_DE_d16();
    uint8_t LD_HL_d16();
    uint8_t LD_SP_d16();

    uint8_t LD_mem_BC_A();
    uint8_t LD_mem_DE_A();
    uint8_t LD_mem_HL_inc_A();
    uint8_t LD_mem_HL_dec_A();

    uint8_t INC_BC();
    uint8_t INC_DE();
    uint8_t INC_HL();
    uint8_t INC_SP();

    uint8_t INC_B();
    uint8_t INC_D();
    uint8_t INC_H();
    uint8_t INC_mem_HL();

    uint8_t DEC_B();
    uint8_t DEC_D();
    uint8_t DEC_H();
    uint8_t DEC_mem_HL();

    uint8_t LD_B_d8();
    uint8_t LD_D_d8();
    uint8_t LD_H_d8();
    uint8_t LD_mem_HL_d8();

    // helper function for 0x01, 0x11, 0x21, 0x31
    uint8_t LD_RR_d16(uint8_t &reg1, uint8_t &reg2);
    // helper function for 0x02, 0x12
    uint8_t LD_mem_RR_A(uint8_t reg1, uint8_t reg2);
    // helper function for 0x05, 0x15, 0x25, 0x35
    uint8_t DEC_R(uint8_t &reg);
    // helper function for 0x06, 0x16, 0x26, 0x36
    uint8_t LD_R_d8(uint8_t &reg);
    // helper function for 0x03, 0x13, 0x23
    uint8_t INC_RR(uint8_t &reg1, uint8_t &reg2);

private:
    // CPU registers
    uint8_t A, F;
    uint8_t B, C, D, E, H, L;
    uint16_t SP;
    uint16_t PC;

    // Memory management component
    Memory memory;

    // CPU Flags
    bool f_Z;
    bool f_N;
    bool f_H;
    bool f_C;

    // Private methods for CPU functionality
    void executeInstruction(uint8_t opcode);
    void add(uint8_t value);
    void sub(uint8_t value);
};

#endif // CPU_H
