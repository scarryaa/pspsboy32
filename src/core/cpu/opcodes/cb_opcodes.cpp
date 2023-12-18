#include "../cpu.h"

// cb helper function for 0x00 - 0x07 (minus 0x06)
uint8_t CPU::RLC_R(uint8_t &reg)
{
    uint8_t result = reg << 1;
    result |= (reg >> 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg >> 7) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x00
uint8_t CPU::RLC_B()
{
    return RLC_R(B);
}

// 0x01
uint8_t CPU::RLC_C()
{
    return RLC_R(C);
}

// 0x02
uint8_t CPU::RLC_D()
{
    return RLC_R(D);
}

// 0x03
uint8_t CPU::RLC_E()
{
    return RLC_R(E);
}

// 0x04
uint8_t CPU::RLC_H()
{
    return RLC_R(H);
}

// 0x05
uint8_t CPU::RLC_L()
{
    return RLC_R(L);
}

// 0x07
uint8_t CPU::RLC_A()
{
    return RLC_R(A);
}

// 0x06
uint8_t CPU::RLC_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value << 1;
    result |= (value >> 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value >> 7) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x08 - 0x0F (minus 0x0E)
uint8_t CPU::RRC_R(uint8_t &reg)
{
    uint8_t result = reg >> 1;
    result |= (reg << 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg & 0x01) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x08
uint8_t CPU::RRC_B()
{
    return RRC_R(B);
}

// 0x09
uint8_t CPU::RRC_C()
{
    return RRC_R(C);
}

// 0x0A
uint8_t CPU::RRC_D()
{
    return RRC_R(D);
}

// 0x0B
uint8_t CPU::RRC_E()
{
    return RRC_R(E);
}

// 0x0C
uint8_t CPU::RRC_H()
{
    return RRC_R(H);
}

// 0x0D
uint8_t CPU::RRC_L()
{
    return RRC_R(L);
}

// 0x0F
uint8_t CPU::RRC_A()
{
    return RRC_R(A);
}

// 0x0E
uint8_t CPU::RRC_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value >> 1;
    result |= (value << 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value & 0x01) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x10 - 0x17 (minus 0x16)
uint8_t CPU::RL_R(uint8_t &reg)
{
    uint8_t result = reg << 1;
    result |= (reg >> 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg >> 7) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x10
uint8_t CPU::RL_B()
{
    return RL_R(B);
}

// 0x11
uint8_t CPU::RL_C()
{
    return RL_R(C);
}

// 0x12
uint8_t CPU::RL_D()
{
    return RL_R(D);
}

// 0x13
uint8_t CPU::RL_E()
{
    return RL_R(E);
}

// 0x14
uint8_t CPU::RL_H()
{
    return RL_R(H);
}

// 0x15
uint8_t CPU::RL_L()
{
    return RL_R(L);
}

// 0x17
uint8_t CPU::RL_A()
{
    return RL_R(A);
}

// 0x16
uint8_t CPU::RL_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value << 1;
    result |= (value >> 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value >> 7) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x18 - 0x1F (minus 0x1E)
uint8_t CPU::RR_R(uint8_t &reg)
{
    uint8_t result = reg >> 1;
    result |= (reg << 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg & 0x01) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x18
uint8_t CPU::RR_B()
{
    return RR_R(B);
}

// 0x19
uint8_t CPU::RR_C()
{
    return RR_R(C);
}

// 0x1A
uint8_t CPU::RR_D()
{
    return RR_R(D);
}

// 0x1B
uint8_t CPU::RR_E()
{
    return RR_R(E);
}

// 0x1C
uint8_t CPU::RR_H()
{
    return RR_R(H);
}

// 0x1D
uint8_t CPU::RR_L()
{
    return RR_R(L);
}

// 0x1F
uint8_t CPU::RR_A()
{
    return RR_R(A);
}

// 0x1E
uint8_t CPU::RR_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value >> 1;
    result |= (value << 7);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value & 0x01) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x20 - 0x27 (minus 0x26)
uint8_t CPU::SLA_R(uint8_t &reg)
{
    uint8_t result = reg << 1;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg >> 7) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x20
uint8_t CPU::SLA_B()
{
    return SLA_R(B);
}

// 0x21
uint8_t CPU::SLA_C()
{
    return SLA_R(C);
}

// 0x22
uint8_t CPU::SLA_D()
{
    return SLA_R(D);
}

// 0x23
uint8_t CPU::SLA_E()
{
    return SLA_R(E);
}

// 0x24
uint8_t CPU::SLA_H()
{
    return SLA_R(H);
}

// 0x25
uint8_t CPU::SLA_L()
{
    return SLA_R(L);
}

// 0x27
uint8_t CPU::SLA_A()
{
    return SLA_R(A);
}

// 0x26
uint8_t CPU::SLA_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value << 1;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value >> 7) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x28 - 0x2F (minus 0x2E)
uint8_t CPU::SRA_R(uint8_t &reg)
{
    uint8_t result = reg >> 1;
    result |= (reg & 0x80);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg & 0x01) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x28
uint8_t CPU::SRA_B()
{
    return SRA_R(B);
}

// 0x29
uint8_t CPU::SRA_C()
{
    return SRA_R(C);
}

// 0x2A
uint8_t CPU::SRA_D()
{
    return SRA_R(D);
}

// 0x2B
uint8_t CPU::SRA_E()
{
    return SRA_R(E);
}

// 0x2C
uint8_t CPU::SRA_H()
{
    return SRA_R(H);
}

// 0x2D
uint8_t CPU::SRA_L()
{
    return SRA_R(L);
}

// 0x2F
uint8_t CPU::SRA_A()
{
    return SRA_R(A);
}

// 0x2E
uint8_t CPU::SRA_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value >> 1;
    result |= (value & 0x80);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value & 0x01) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x30 - 0x37 (minus 0x36)
uint8_t CPU::SWAP_R(uint8_t &reg)
{
    uint8_t result = ((reg & 0x0F) << 4) | ((reg & 0xF0) >> 4);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, false);

    reg = result;

    PC += 2;
    return 8;
}

// 0x30
uint8_t CPU::SWAP_B()
{
    return SWAP_R(B);
}

// 0x31
uint8_t CPU::SWAP_C()
{
    return SWAP_R(C);
}

// 0x32
uint8_t CPU::SWAP_D()
{
    return SWAP_R(D);
}

// 0x33
uint8_t CPU::SWAP_E()
{
    return SWAP_R(E);
}

// 0x34
uint8_t CPU::SWAP_H()
{
    return SWAP_R(H);
}

// 0x35
uint8_t CPU::SWAP_L()
{
    return SWAP_R(L);
}

// 0x37
uint8_t CPU::SWAP_A()
{
    return SWAP_R(A);
}

// 0x36
uint8_t CPU::SWAP_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, false);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x38 - 0x3F (minus 0x3E)
uint8_t CPU::SRL_R(uint8_t &reg)
{
    uint8_t result = reg >> 1;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (reg & 0x01) == 1);

    reg = result;

    PC += 2;
    return 8;
}

// 0x38
uint8_t CPU::SRL_B()
{
    return SRL_R(B);
}

// 0x39
uint8_t CPU::SRL_C()
{
    return SRL_R(C);
}

// 0x3A
uint8_t CPU::SRL_D()
{
    return SRL_R(D);
}

// 0x3B
uint8_t CPU::SRL_E()
{
    return SRL_R(E);
}

// 0x3C
uint8_t CPU::SRL_H()
{
    return SRL_R(H);
}

// 0x3D
uint8_t CPU::SRL_L()
{
    return SRL_R(L);
}

// 0x3F
uint8_t CPU::SRL_A()
{
    return SRL_R(A);
}

// 0x3E
uint8_t CPU::SRL_HL()
{
    uint8_t value = memory.readByte(HL);
    uint8_t result = value >> 1;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, (value & 0x01) == 1);

    memory.writeByte(HL, result);

    PC += 2;
    return 16;
}

// cb helper function for 0x40 - 0x7F
uint8_t CPU::BIT_b_R(uint8_t bit, uint8_t reg)
{
    SET_FLAG_Z(CPU::F, (reg & (1 << bit)) == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, true);

    PC += 2;
    return 8;
}

// 0x40
uint8_t CPU::BIT_0_B()
{
    return BIT_b_R(0, B);
}

// 0x41
uint8_t CPU::BIT_0_C()
{
    return BIT_b_R(0, C);
}

// 0x42
uint8_t CPU::BIT_0_D()
{
    return BIT_b_R(0, D);
}

// 0x43
uint8_t CPU::BIT_0_E()
{
    return BIT_b_R(0, E);
}

// 0x44
uint8_t CPU::BIT_0_H()
{
    return BIT_b_R(0, H);
}

// 0x45
uint8_t CPU::BIT_0_L()
{
    return BIT_b_R(0, L);
}

// 0x47
uint8_t CPU::BIT_0_A()
{
    return BIT_b_R(0, A);
}

// 0x46
uint8_t CPU::BIT_0_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(0, value);
}

// 0x48
uint8_t CPU::BIT_1_B()
{
    return BIT_b_R(1, B);
}

// 0x49
uint8_t CPU::BIT_1_C()
{
    return BIT_b_R(1, C);
}

// 0x4A
uint8_t CPU::BIT_1_D()
{
    return BIT_b_R(1, D);
}

// 0x4B
uint8_t CPU::BIT_1_E()
{
    return BIT_b_R(1, E);
}

// 0x4C
uint8_t CPU::BIT_1_H()
{
    return BIT_b_R(1, H);
}

// 0x4D
uint8_t CPU::BIT_1_L()
{
    return BIT_b_R(1, L);
}

// 0x4F
uint8_t CPU::BIT_1_A()
{
    return BIT_b_R(1, A);
}

// 0x4E
uint8_t CPU::BIT_1_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(1, value);
}

// 0x50
uint8_t CPU::BIT_2_B()
{
    return BIT_b_R(2, B);
}

// 0x51
uint8_t CPU::BIT_2_C()
{
    return BIT_b_R(2, C);
}

// 0x52
uint8_t CPU::BIT_2_D()
{
    return BIT_b_R(2, D);
}

// 0x53
uint8_t CPU::BIT_2_E()
{
    return BIT_b_R(2, E);
}

// 0x54
uint8_t CPU::BIT_2_H()
{
    return BIT_b_R(2, H);
}

// 0x55
uint8_t CPU::BIT_2_L()
{
    return BIT_b_R(2, L);
}

// 0x57
uint8_t CPU::BIT_2_A()
{
    return BIT_b_R(2, A);
}

// 0x56
uint8_t CPU::BIT_2_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(2, value);
}

// 0x58
uint8_t CPU::BIT_3_B()
{
    return BIT_b_R(3, B);
}

// 0x59
uint8_t CPU::BIT_3_C()
{
    return BIT_b_R(3, C);
}

// 0x5A
uint8_t CPU::BIT_3_D()
{
    return BIT_b_R(3, D);
}

// 0x5B
uint8_t CPU::BIT_3_E()
{
    return BIT_b_R(3, E);
}

// 0x5C
uint8_t CPU::BIT_3_H()
{
    return BIT_b_R(3, H);
}

// 0x5D
uint8_t CPU::BIT_3_L()
{
    return BIT_b_R(3, L);
}

// 0x5F
uint8_t CPU::BIT_3_A()
{
    return BIT_b_R(3, A);
}

// 0x5E
uint8_t CPU::BIT_3_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(3, value);
}

// 0x60
uint8_t CPU::BIT_4_B()
{
    return BIT_b_R(4, B);
}

// 0x61
uint8_t CPU::BIT_4_C()
{
    return BIT_b_R(4, C);
}

// 0x62
uint8_t CPU::BIT_4_D()
{
    return BIT_b_R(4, D);
}

// 0x63
uint8_t CPU::BIT_4_E()
{
    return BIT_b_R(4, E);
}

// 0x64
uint8_t CPU::BIT_4_H()
{
    return BIT_b_R(4, H);
}

// 0x65
uint8_t CPU::BIT_4_L()
{
    return BIT_b_R(4, L);
}

// 0x67
uint8_t CPU::BIT_4_A()
{
    return BIT_b_R(4, A);
}

// 0x66
uint8_t CPU::BIT_4_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(4, value);
}

// 0x68
uint8_t CPU::BIT_5_B()
{
    return BIT_b_R(5, B);
}

// 0x69
uint8_t CPU::BIT_5_C()
{
    return BIT_b_R(5, C);
}

// 0x6A
uint8_t CPU::BIT_5_D()
{
    return BIT_b_R(5, D);
}

// 0x6B
uint8_t CPU::BIT_5_E()
{
    return BIT_b_R(5, E);
}

// 0x6C
uint8_t CPU::BIT_5_H()
{
    return BIT_b_R(5, H);
}

// 0x6D
uint8_t CPU::BIT_5_L()
{
    return BIT_b_R(5, L);
}

// 0x6F
uint8_t CPU::BIT_5_A()
{
    return BIT_b_R(5, A);
}

// 0x6E
uint8_t CPU::BIT_5_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(5, value);
}

// 0x70
uint8_t CPU::BIT_6_B()
{
    return BIT_b_R(6, B);
}

// 0x71
uint8_t CPU::BIT_6_C()
{
    return BIT_b_R(6, C);
}

// 0x72
uint8_t CPU::BIT_6_D()
{
    return BIT_b_R(6, D);
}

// 0x73
uint8_t CPU::BIT_6_E()
{
    return BIT_b_R(6, E);
}

// 0x74
uint8_t CPU::BIT_6_H()
{
    return BIT_b_R(6, H);
}

// 0x75
uint8_t CPU::BIT_6_L()
{
    return BIT_b_R(6, L);
}

// 0x77
uint8_t CPU::BIT_6_A()
{
    return BIT_b_R(6, A);
}

// 0x76
uint8_t CPU::BIT_6_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(6, value);
}

// 0x78
uint8_t CPU::BIT_7_B()
{
    return BIT_b_R(7, B);
}

// 0x79
uint8_t CPU::BIT_7_C()
{
    return BIT_b_R(7, C);
}

// 0x7A
uint8_t CPU::BIT_7_D()
{
    return BIT_b_R(7, D);
}

// 0x7B
uint8_t CPU::BIT_7_E()
{
    return BIT_b_R(7, E);
}

// 0x7C
uint8_t CPU::BIT_7_H()
{
    return BIT_b_R(7, H);
}

// 0x7D
uint8_t CPU::BIT_7_L()
{
    return BIT_b_R(7, L);
}

// 0x7F
uint8_t CPU::BIT_7_A()
{
    return BIT_b_R(7, A);
}

// 0x7E
uint8_t CPU::BIT_7_HL()
{
    uint8_t value = memory.readByte(HL);
    return BIT_b_R(7, value);
}

// cb helper function for 0x80 - 0xBF
uint8_t CPU::RES_b_R(uint8_t bit, uint8_t &reg)
{
    reg &= ~(1 << bit);

    PC += 2;
    return 8;
}

// 0x80
uint8_t CPU::RES_0_B()
{
    return RES_b_R(0, B);
}

// 0x81
uint8_t CPU::RES_0_C()
{
    return RES_b_R(0, C);
}

// 0x82
uint8_t CPU::RES_0_D()
{
    return RES_b_R(0, D);
}

// 0x83
uint8_t CPU::RES_0_E()
{
    return RES_b_R(0, E);
}

// 0x84
uint8_t CPU::RES_0_H()
{
    return RES_b_R(0, H);
}

// 0x85
uint8_t CPU::RES_0_L()
{
    return RES_b_R(0, L);
}

// 0x87
uint8_t CPU::RES_0_A()
{
    return RES_b_R(0, A);
}

// 0x86
uint8_t CPU::RES_0_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(0, value);
}

// 0x88
uint8_t CPU::RES_1_B()
{
    return RES_b_R(1, B);
}

// 0x89
uint8_t CPU::RES_1_C()
{
    return RES_b_R(1, C);
}

// 0x8A
uint8_t CPU::RES_1_D()
{
    return RES_b_R(1, D);
}

// 0x8B
uint8_t CPU::RES_1_E()
{
    return RES_b_R(1, E);
}

// 0x8C
uint8_t CPU::RES_1_H()
{
    return RES_b_R(1, H);
}

// 0x8D
uint8_t CPU::RES_1_L()
{
    return RES_b_R(1, L);
}

// 0x8F
uint8_t CPU::RES_1_A()
{
    return RES_b_R(1, A);
}

// 0x8E
uint8_t CPU::RES_1_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(1, value);
}

// 0x90
uint8_t CPU::RES_2_B()
{
    return RES_b_R(2, B);
}

// 0x91
uint8_t CPU::RES_2_C()
{
    return RES_b_R(2, C);
}

// 0x92
uint8_t CPU::RES_2_D()
{
    return RES_b_R(2, D);
}

// 0x93
uint8_t CPU::RES_2_E()
{
    return RES_b_R(2, E);
}

// 0x94
uint8_t CPU::RES_2_H()
{
    return RES_b_R(2, H);
}

// 0x95
uint8_t CPU::RES_2_L()
{
    return RES_b_R(2, L);
}

// 0x97
uint8_t CPU::RES_2_A()
{
    return RES_b_R(2, A);
}

// 0x96
uint8_t CPU::RES_2_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(2, value);
}

// 0x98
uint8_t CPU::RES_3_B()
{
    return RES_b_R(3, B);
}

// 0x99
uint8_t CPU::RES_3_C()
{
    return RES_b_R(3, C);
}

// 0x9A
uint8_t CPU::RES_3_D()
{
    return RES_b_R(3, D);
}

// 0x9B
uint8_t CPU::RES_3_E()
{
    return RES_b_R(3, E);
}

// 0x9C
uint8_t CPU::RES_3_H()
{
    return RES_b_R(3, H);
}

// 0x9D
uint8_t CPU::RES_3_L()
{
    return RES_b_R(3, L);
}

// 0x9F
uint8_t CPU::RES_3_A()
{
    return RES_b_R(3, A);
}

// 0x9E
uint8_t CPU::RES_3_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(3, value);
}

// 0xA0
uint8_t CPU::RES_4_B()
{
    return RES_b_R(4, B);
}

// 0xA1
uint8_t CPU::RES_4_C()
{
    return RES_b_R(4, C);
}

// 0xA2
uint8_t CPU::RES_4_D()
{
    return RES_b_R(4, D);
}

// 0xA3
uint8_t CPU::RES_4_E()
{
    return RES_b_R(4, E);
}

// 0xA4
uint8_t CPU::RES_4_H()
{
    return RES_b_R(4, H);
}

// 0xA5
uint8_t CPU::RES_4_L()
{
    return RES_b_R(4, L);
}

// 0xA7
uint8_t CPU::RES_4_A()
{
    return RES_b_R(4, A);
}

// 0xA6
uint8_t CPU::RES_4_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(4, value);
}

// 0xA8
uint8_t CPU::RES_5_B()
{
    return RES_b_R(5, B);
}

// 0xA9
uint8_t CPU::RES_5_C()
{
    return RES_b_R(5, C);
}

// 0xAA
uint8_t CPU::RES_5_D()
{
    return RES_b_R(5, D);
}

// 0xAB
uint8_t CPU::RES_5_E()
{
    return RES_b_R(5, E);
}

// 0xAC
uint8_t CPU::RES_5_H()
{
    return RES_b_R(5, H);
}

// 0xAD
uint8_t CPU::RES_5_L()
{
    return RES_b_R(5, L);
}

// 0xAF
uint8_t CPU::RES_5_A()
{
    return RES_b_R(5, A);
}

// 0xAE
uint8_t CPU::RES_5_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(5, value);
}

// 0xB0
uint8_t CPU::RES_6_B()
{
    return RES_b_R(6, B);
}

// 0xB1
uint8_t CPU::RES_6_C()
{
    return RES_b_R(6, C);
}

// 0xB2
uint8_t CPU::RES_6_D()
{
    return RES_b_R(6, D);
}

// 0xB3
uint8_t CPU::RES_6_E()
{
    return RES_b_R(6, E);
}

// 0xB4
uint8_t CPU::RES_6_H()
{
    return RES_b_R(6, H);
}

// 0xB5
uint8_t CPU::RES_6_L()
{
    return RES_b_R(6, L);
}

// 0xB7
uint8_t CPU::RES_6_A()
{
    return RES_b_R(6, A);
}

// 0xB6
uint8_t CPU::RES_6_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(6, value);
}

// 0xB8
uint8_t CPU::RES_7_B()
{
    return RES_b_R(7, B);
}

// 0xB9
uint8_t CPU::RES_7_C()
{
    return RES_b_R(7, C);
}

// 0xBA
uint8_t CPU::RES_7_D()
{
    return RES_b_R(7, D);
}

// 0xBB
uint8_t CPU::RES_7_E()
{
    return RES_b_R(7, E);
}

// 0xBC
uint8_t CPU::RES_7_H()
{
    return RES_b_R(7, H);
}

// 0xBD
uint8_t CPU::RES_7_L()
{
    return RES_b_R(7, L);
}

// 0xBF
uint8_t CPU::RES_7_A()
{
    return RES_b_R(7, A);
}

// 0xBE
uint8_t CPU::RES_7_HL()
{
    uint8_t value = memory.readByte(HL);
    return RES_b_R(7, value);
}

// cb helper function for 0xC0 - 0xFF
uint8_t CPU::SET_b_R(uint8_t bit, uint8_t &reg)
{
    reg |= (1 << bit);

    PC += 2;
    return 8;
}

// 0xC0
uint8_t CPU::SET_0_B()
{
    return SET_b_R(0, B);
}

// 0xC1
uint8_t CPU::SET_0_C()
{
    return SET_b_R(0, C);
}

// 0xC2
uint8_t CPU::SET_0_D()
{
    return SET_b_R(0, D);
}

// 0xC3
uint8_t CPU::SET_0_E()
{
    return SET_b_R(0, E);
}

// 0xC4
uint8_t CPU::SET_0_H()
{
    return SET_b_R(0, H);
}

// 0xC5
uint8_t CPU::SET_0_L()
{
    return SET_b_R(0, L);
}

// 0xC7
uint8_t CPU::SET_0_A()
{
    return SET_b_R(0, A);
}

// 0xC6
uint8_t CPU::SET_0_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(0, value);
}

// 0xC8
uint8_t CPU::SET_1_B()
{
    return SET_b_R(1, B);
}

// 0xC9
uint8_t CPU::SET_1_C()
{
    return SET_b_R(1, C);
}

// 0xCA
uint8_t CPU::SET_1_D()
{
    return SET_b_R(1, D);
}

// 0xCB
uint8_t CPU::SET_1_E()
{
    return SET_b_R(1, E);
}

// 0xCC
uint8_t CPU::SET_1_H()
{
    return SET_b_R(1, H);
}

// 0xCD
uint8_t CPU::SET_1_L()
{
    return SET_b_R(1, L);
}

// 0xCF
uint8_t CPU::SET_1_A()
{
    return SET_b_R(1, A);
}

// 0xCE
uint8_t CPU::SET_1_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(1, value);
}

// 0xD0
uint8_t CPU::SET_2_B()
{
    return SET_b_R(2, B);
}

// 0xD1
uint8_t CPU::SET_2_C()
{
    return SET_b_R(2, C);
}

// 0xD2
uint8_t CPU::SET_2_D()
{
    return SET_b_R(2, D);
}

// 0xD3
uint8_t CPU::SET_2_E()
{
    return SET_b_R(2, E);
}

// 0xD4
uint8_t CPU::SET_2_H()
{
    return SET_b_R(2, H);
}

// 0xD5
uint8_t CPU::SET_2_L()
{
    return SET_b_R(2, L);
}

// 0xD7
uint8_t CPU::SET_2_A()
{
    return SET_b_R(2, A);
}

// 0xD6
uint8_t CPU::SET_2_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(2, value);
}

// 0xD8
uint8_t CPU::SET_3_B()
{
    return SET_b_R(3, B);
}

// 0xD9
uint8_t CPU::SET_3_C()
{
    return SET_b_R(3, C);
}

// 0xDA
uint8_t CPU::SET_3_D()
{
    return SET_b_R(3, D);
}

// 0xDB
uint8_t CPU::SET_3_E()
{
    return SET_b_R(3, E);
}

// 0xDC
uint8_t CPU::SET_3_H()
{
    return SET_b_R(3, H);
}

// 0xDD
uint8_t CPU::SET_3_L()
{
    return SET_b_R(3, L);
}

// 0xDF
uint8_t CPU::SET_3_A()
{
    return SET_b_R(3, A);
}

// 0xDE
uint8_t CPU::SET_3_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(3, value);
}

// 0xE0
uint8_t CPU::SET_4_B()
{
    return SET_b_R(4, B);
}

// 0xE1
uint8_t CPU::SET_4_C()
{
    return SET_b_R(4, C);
}

// 0xE2
uint8_t CPU::SET_4_D()
{
    return SET_b_R(4, D);
}

// 0xE3
uint8_t CPU::SET_4_E()
{
    return SET_b_R(4, E);
}

// 0xE4
uint8_t CPU::SET_4_H()
{
    return SET_b_R(4, H);
}

// 0xE5
uint8_t CPU::SET_4_L()
{
    return SET_b_R(4, L);
}

// 0xE7
uint8_t CPU::SET_4_A()
{
    return SET_b_R(4, A);
}

// 0xE6
uint8_t CPU::SET_4_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(4, value);
}

// 0xE8
uint8_t CPU::SET_5_B()
{
    return SET_b_R(5, B);
}

// 0xE9
uint8_t CPU::SET_5_C()
{
    return SET_b_R(5, C);
}

// 0xEA
uint8_t CPU::SET_5_D()
{
    return SET_b_R(5, D);
}

// 0xEB
uint8_t CPU::SET_5_E()
{
    return SET_b_R(5, E);
}

// 0xEC
uint8_t CPU::SET_5_H()
{
    return SET_b_R(5, H);
}

// 0xED
uint8_t CPU::SET_5_L()
{
    return SET_b_R(5, L);
}

// 0xEF
uint8_t CPU::SET_5_A()
{
    return SET_b_R(5, A);
}

// 0xEE
uint8_t CPU::SET_5_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(5, value);
}

// 0xF0
uint8_t CPU::SET_6_B()
{
    return SET_b_R(6, B);
}

// 0xF1
uint8_t CPU::SET_6_C()
{
    return SET_b_R(6, C);
}

// 0xF2
uint8_t CPU::SET_6_D()
{
    return SET_b_R(6, D);
}

// 0xF3
uint8_t CPU::SET_6_E()
{
    return SET_b_R(6, E);
}

// 0xF4
uint8_t CPU::SET_6_H()
{
    return SET_b_R(6, H);
}

// 0xF5
uint8_t CPU::SET_6_L()
{
    return SET_b_R(6, L);
}

// 0xF7
uint8_t CPU::SET_6_A()
{
    return SET_b_R(6, A);
}

// 0xF6
uint8_t CPU::SET_6_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(6, value);
}

// 0xF8
uint8_t CPU::SET_7_B()
{
    return SET_b_R(7, B);
}

// 0xF9
uint8_t CPU::SET_7_C()
{
    return SET_b_R(7, C);
}

// 0xFA
uint8_t CPU::SET_7_D()
{
    return SET_b_R(7, D);
}

// 0xFB
uint8_t CPU::SET_7_E()
{
    return SET_b_R(7, E);
}

// 0xFC
uint8_t CPU::SET_7_H()
{
    return SET_b_R(7, H);
}

// 0xFD
uint8_t CPU::SET_7_L()
{
    return SET_b_R(7, L);
}

// 0xFF
uint8_t CPU::SET_7_A()
{
    return SET_b_R(7, A);
}

// 0xFE
uint8_t CPU::SET_7_HL()
{
    uint8_t value = memory.readByte(HL);
    return SET_b_R(7, value);
}