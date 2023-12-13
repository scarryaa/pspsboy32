#include "../cpu.h"

// 0x00
uint8_t CPU::NoOperation() {
    PC += 1;
    return 4;
}

// 0x10
uint8_t CPU::Stop() {
    stopped = true;
    PC += 2;
    return 4;
}

// 0x20
uint8_t CPU::JR_NZ_i8() {
    if (!f_Z) {
        int8_t offset = static_cast<int8_t>(memory.readByte(PC + 1));
        PC += offset;
        return 12;
    }

    PC += 2;
    return 8;
}

// 0x30
uint8_t CPU::JR_NC_i8() {
    if (!f_C) {
        int8_t offset = static_cast<int8_t>(memory.readByte(PC + 1));
        PC += offset;
        return 12;
    }

    PC += 2;
    return 8;
}

// helper function for 0x01, 0x11, 0x21
uint8_t CPU::LD_RR_d16(uint8_t &reg1, uint8_t &reg2) {
    reg1 = memory.readByte(PC + 2);
    reg2 = memory.readByte(PC + 1);
    PC += 3;
    return 12;
}

// 0x01
uint8_t CPU::LD_BC_d16() {
    return LD_RR_d16(B, C);
}

// 0x11
uint8_t CPU::LD_DE_d16() {
    return LD_RR_d16(D, E);
}

// 0x21
uint8_t CPU::LD_HL_d16() {
    return LD_RR_d16(H, L);
}

// 0x31
uint8_t CPU::LD_SP_d16() {
    SP = memory.readByte(PC + 2);
    SP = (SP << 8) | memory.readByte(PC + 1);
    PC += 3;
    return 12;
}

// helper function for 0x02, 0x12
uint8_t CPU::LD_mem_RR_A(uint8_t reg1, uint8_t reg2) {
    memory.writeByte((reg1 << 8) | reg2, A);
    PC += 1;
    return 8;
}

// 0x02
uint8_t CPU::LD_mem_BC_A() {
    return LD_mem_RR_A(B, C);
}

// 0x12
uint8_t CPU::LD_mem_DE_A() {
    return LD_mem_RR_A(D, E);
}

// 0x22
uint8_t CPU::LD_mem_HL_inc_A() {
    memory.writeByte((H << 8) | L, A);
    L += 1;
    PC += 1;
    return 8;
}

// 0x32
uint8_t CPU::LD_mem_HL_dec_A() {
    memory.writeByte((H << 8) | L, A);
    L -= 1;
    PC += 1;
    return 8;
}

// helper function for 0x03, 0x13, 0x23
uint8_t CPU::INC_RR(uint8_t &reg1, uint8_t &reg2) {
    uint16_t reg = (reg1 << 8) | reg2;
    reg += 1;
    reg1 = (reg >> 8) & 0xFF;
    reg2 = reg & 0xFF;
    PC += 1;
    return 8;
}

// 0x03
uint8_t CPU::INC_BC() {
    return INC_RR(B, C);
}

// 0x13
uint8_t CPU::INC_DE() {
    return INC_RR(D, E);
}

// 0x23
uint8_t CPU::INC_HL() {
    return INC_RR(H, L);
}

// 0x33
uint8_t CPU::INC_SP() {
    SP += 1;
    PC += 1;
    return 8;
}

// 0x04
uint8_t CPU::INC_B() {
    B += 1;

    f_Z = (B == 0);
    f_N = false;
    f_H = ((B & 0x0F) == 0);

    PC += 1;
    return 4;
}

// 0x14
uint8_t CPU::INC_D() {
    D += 1;

    f_Z = (D == 0);
    f_N = false;
    f_H = ((D & 0x0F) == 0);

    PC += 1;
    return 4;
}

// 0x24
uint8_t CPU::INC_H() {
    H += 1;

    f_Z = (H == 0);
    f_N = false;
    f_H = ((H & 0x0F) == 0);

    PC += 1;
    return 4;
}

// 0x34
uint8_t CPU::INC_mem_HL() {
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    value += 1;
    memory.writeByte(HL, value);

    f_Z = (value == 0);
    f_N = false;
    f_H = ((value & 0x0F) == 0);

    PC += 1;
    return 12;
}

// helper function for 0x05, 0x15, 0x25, 0x35
uint8_t CPU::DEC_R(uint8_t &reg) {
    reg -= 1;

    f_Z = (reg == 0);
    f_N = true;
    f_H = ((reg & 0x0F) == 0x0F);

    PC += 1;
    return 4;
}

// 0x05
uint8_t CPU::DEC_B() {
    return DEC_R(B);
}

// 0x15
uint8_t CPU::DEC_D() {
    return DEC_R(D);
}

// 0x25
uint8_t CPU::DEC_H() {
    return DEC_R(H);
}

// 0x35
uint8_t CPU::DEC_mem_HL() {
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    value -= 1;
    memory.writeByte(HL, value);

    f_Z = (value == 0);
    f_N = true;
    f_H = ((value & 0x0F) == 0x0F);

    PC += 1;
    return 12;
}

// helper function for 0x06, 0x16, 0x26, 0x36
uint8_t CPU::LD_R_d8(uint8_t &reg) {
    reg = memory.readByte(PC + 1);
    PC += 2;
    return 8;
}

// 0x06
uint8_t CPU::LD_B_d8() {
    return LD_R_d8(B);
}

// 0x16
uint8_t CPU::LD_D_d8() {
    return LD_R_d8(D);
}

// 0x26
uint8_t CPU::LD_H_d8() {
    return LD_R_d8(H);
}

// 0x36
uint8_t CPU::LD_mem_HL_d8() {
    uint16_t HL = (H << 8) | L;
    memory.writeByte(HL, memory.readByte(PC + 1));
    PC += 2;
    return 12;
}