#include "../cpu.h"

// INVALID
uint8_t CPU::INVALID_D3()
{
    printf("INVALID OPCODE 0xD3\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_E3()
{
    printf("INVALID OPCODE 0xE3\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_E4()
{
    printf("INVALID OPCODE 0xE4\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_F4()
{
    printf("INVALID OPCODE 0xF4\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_DB()
{
    printf("INVALID OPCODE 0xDB\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_EB()
{
    printf("INVALID OPCODE 0xEB\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_EC()
{
    printf("INVALID OPCODE 0xEC\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_FC()
{
    printf("INVALID OPCODE 0xFC\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_DD()
{
    printf("INVALID OPCODE 0xDD\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_ED()
{
    printf("INVALID OPCODE 0xED\n");
    return 0;
}

// INVALID
uint8_t CPU::INVALID_FD()
{
    printf("INVALID OPCODE 0xFD\n");
    return 0;
}

// 0x00
uint8_t CPU::NoOperation()
{
    return 4;
}

// 0x10
uint8_t CPU::Stop()
{
    // halted = true;

    // fetch instruction but dont do anything with it
    fetchInstruction();
    return 4;
}

// helper function for 0x20, 0x30, 0x28, 0x38
uint8_t CPU::JR_i8(uint8_t condition)
{
    int8_t offset = static_cast<int8_t>(fetchInstruction());

    if (condition)
    {
        PC += offset;
        return 12;
    }
    else
    {
        return 8;
    }
}

// 0x20
uint8_t CPU::JR_NZ_i8()
{
    return JR_i8(!GET_FLAG_Z(CPU::F));
}

// 0x30
uint8_t CPU::JR_NC_i8()
{
    return JR_i8(!GET_FLAG_C(CPU::F));
}

// helper function for 0x01, 0x11, 0x21
uint8_t CPU::LD_RR_d16(uint8_t &reg1, uint8_t &reg2)
{
    reg2 = fetchInstruction();
    reg1 = fetchInstruction();
    return 12;
}

// 0x01
uint8_t CPU::LD_BC_d16()
{
    return LD_RR_d16(B, C);
}

// 0x11
uint8_t CPU::LD_DE_d16()
{
    return LD_RR_d16(D, E);
}

// 0x21
uint8_t CPU::LD_HL_d16()
{
    return LD_RR_d16(H, L);
}

// 0x31
uint8_t CPU::LD_SP_d16()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    SP = (high << 8) | low;
    return 12;
}

// helper function for 0x02, 0x12
uint8_t CPU::LD_mem_RR_A(uint8_t reg1, uint8_t reg2)
{
    memory.writeByte((reg1 << 8) | reg2, A);
    return 8;
}

// 0x02
uint8_t CPU::LD_mem_BC_A()
{
    return LD_mem_RR_A(B, C);
}

// 0x12
uint8_t CPU::LD_mem_DE_A()
{
    return LD_mem_RR_A(D, E);
}

// 0x22
uint8_t CPU::LD_mem_HL_inc_A()
{
    uint16_t HL = (H << 8) | L;
    memory.writeByte(HL, A);
    HL += 1;
    H = (HL >> 8) & 0xFF;
    L = HL & 0xFF;
    return 8;
}

// 0x32
uint8_t CPU::LD_mem_HL_dec_A()
{
    memory.writeByte((H << 8) | L, A);

    if (L == 0)
    {
        H--;
    }
    L--;

    return 8;
}

// helper function for 0x03, 0x13, 0x23
uint8_t CPU::INC_RR(uint8_t &reg1, uint8_t &reg2)
{
    uint16_t reg = (reg1 << 8) | reg2;
    reg += 1;

    reg1 = (reg >> 8) & 0xFF;
    reg2 = reg & 0xFF;

    return 8;
}

// 0x03
uint8_t CPU::INC_BC()
{
    return INC_RR(B, C);
}

// 0x13
uint8_t CPU::INC_DE()
{
    return INC_RR(D, E);
}

// 0x23
uint8_t CPU::INC_HL()
{
    return INC_RR(H, L);
}

// 0x33
uint8_t CPU::INC_SP()
{
    SP += 1;
    return 8;
}

uint8_t CPU::INC_B()
{
    B += 1;

    SET_FLAG_Z(CPU::F, B == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (B & 0x0F) == 0x00);

    return 4;
}

// 0x14
uint8_t CPU::INC_D()
{
    D += 1;

    SET_FLAG_Z(CPU::F, D == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (D & 0x0F) == 0x00);

    return 4;
}

// 0x24
uint8_t CPU::INC_H()
{
    H += 1;

    SET_FLAG_Z(CPU::F, H == 0);
    SET_FLAG_N(CPU::F, false);

    SET_FLAG_H(CPU::F, (H & 0x0F) == 0x00);

    return 4;
}

// 0x34
uint8_t CPU::INC_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    value += 1;
    memory.writeByte(HL, value);

    SET_FLAG_Z(CPU::F, value == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (value & 0x0F) == 0);

    return 12;
}

// helper function for 0x05, 0x15, 0x25, 0x35
uint8_t CPU::DEC_R(uint8_t &reg)
{
    SET_FLAG_H(CPU::F, (reg & 0x0F) == 0);

    reg -= 1;

    SET_FLAG_Z(CPU::F, reg == 0);
    SET_FLAG_N(CPU::F, true);

    return 4;
}

// 0x05
uint8_t CPU::DEC_B()
{
    return DEC_R(B);
}

// 0x15
uint8_t CPU::DEC_D()
{
    return DEC_R(D);
}

// 0x25
uint8_t CPU::DEC_H()
{
    return DEC_R(H);
}

uint8_t CPU::DEC_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    uint8_t newValue = value - 1;
    memory.writeByte(HL, newValue);

    SET_FLAG_Z(CPU::F, newValue == 0);
    SET_FLAG_N(CPU::F, true);
    SET_FLAG_H(CPU::F, (value & 0x0F) == 0);

    return 12;
}

// helper function for 0x06, 0x16, 0x26, 0x36
uint8_t CPU::LD_R_d8(uint8_t &reg)
{
    reg = fetchInstruction();
    return 8;
}

// 0x06
uint8_t CPU::LD_B_d8()
{
    return LD_R_d8(B);
}

// 0x16
uint8_t CPU::LD_D_d8()
{
    return LD_R_d8(D);
}

// 0x26
uint8_t CPU::LD_H_d8()
{
    return LD_R_d8(H);
}

// 0x36
uint8_t CPU::LD_mem_HL_d8()
{
    uint16_t HL = (H << 8) | L;
    memory.writeByte(HL, fetchInstruction());
    return 12;
}

// 0x07
uint8_t CPU::RLCA()
{
    uint8_t bit7 = (A & 0x80) >> 7;
    A = (A << 1) | bit7;

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, bit7 == 1);

    return 4;
}

// 0x17
uint8_t CPU::RLA()
{
    uint8_t bit7 = (A & 0x80) >> 7;
    A = (A << 1) | GET_FLAG_C(CPU::F);

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, bit7 == 1);

    return 4;
}

// 0x27 DAA
uint8_t CPU::DAA()
{
    uint16_t correction = 0;
    bool set_carry = GET_FLAG_C(CPU::F);

    // Adjust for half-carry or if lower nibble of A is greater than 9
    if (GET_FLAG_H(CPU::F) || (!GET_FLAG_N(CPU::F) && (A & 0x0F) > 9))
        correction += 0x06;

    // Adjust for carry or if A is greater than 99
    if (set_carry || (!GET_FLAG_N(CPU::F) && A > 0x99))
    {
        correction += 0x60;
        set_carry = true;
    }

    // Adjust A based on the subtraction flag
    A += (GET_FLAG_N(CPU::F) ? -correction : correction);

    // Set or reset flags based on results
    SET_FLAG_Z(CPU::F, (A == 0));
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, set_carry);

    return 4;
}

// 0x37
uint8_t CPU::SCF()
{
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, true);

    return 4;
}

// 0x08
uint8_t CPU::LD_mem_d16_SP()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    uint16_t address = (high << 8) | low;
    memory.writeByte(address, SP & 0xFF);
    memory.writeByte(address + 1, SP >> 8);
    return 20;
}

// 0x18
uint8_t CPU::JR_i8()
{
    int8_t offset = static_cast<int8_t>(fetchInstruction());
    PC += offset;
    return 12;
}

// 0x28
uint8_t CPU::JR_Z_i8()
{
    return JR_i8(GET_FLAG_Z(CPU::F));
}

// 0x38
uint8_t CPU::JR_C_i8()
{
    return JR_i8(GET_FLAG_C(CPU::F));
}

// helper function for 0x09, 0x19, 0x29
uint8_t CPU::ADD_HL_RR(uint8_t reg1, uint8_t reg2)
{
    uint16_t HL = (H << 8) | L;
    uint16_t reg = (reg1 << 8) | reg2;
    uint32_t result = HL + reg;

    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, ((HL & 0x0FFF) + (reg & 0x0FFF) > 0x0FFF));
    SET_FLAG_C(CPU::F, result > 0xFFFF);

    H = (result >> 8) & 0xFF;
    L = result & 0xFF;

    return 8;
}

// 0x09
uint8_t CPU::ADD_HL_BC()
{
    return ADD_HL_RR(B, C);
}

// 0x19
uint8_t CPU::ADD_HL_DE()
{
    return ADD_HL_RR(D, E);
}

// 0x29
uint8_t CPU::ADD_HL_HL()
{
    return ADD_HL_RR(H, L);
}

// 0x39
uint8_t CPU::ADD_HL_SP()
{
    uint16_t HL = (H << 8) | L;
    uint32_t result = HL + SP;

    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, ((HL & 0x0FFF) + (SP & 0x0FFF) > 0x0FFF));
    SET_FLAG_C(CPU::F, ((uint32_t)HL + SP) > 0xFFFF);

    H = (result >> 8) & 0xFF;
    L = result & 0xFF;

    return 8;
}

// helper function for 0x0A, 0x1A
uint8_t CPU::LD_A_mem_RR(uint8_t reg1, uint8_t reg2)
{
    A = memory.readByte((reg1 << 8) | reg2);
    return 8;
}

// 0x0A
uint8_t CPU::LD_A_mem_BC()
{
    return LD_A_mem_RR(B, C);
}

// 0x1A
uint8_t CPU::LD_A_mem_DE()
{
    return LD_A_mem_RR(D, E);
}

// helper function for 0x2A, 0x3A
uint8_t CPU::LD_A_mem_HL_inc_dec(int8_t inc)
{
    uint16_t HL = (H << 8) | L;
    A = memory.readByte(HL);

    HL = static_cast<uint16_t>(HL + inc);

    H = (HL >> 8) & 0xFF;
    L = HL & 0xFF;
    return 8;
}

// 0x2A
uint8_t CPU::LD_A_mem_HL_inc()
{
    return LD_A_mem_HL_inc_dec(1);
}

// 0x3A
uint8_t CPU::LD_A_mem_HL_dec()
{
    return LD_A_mem_HL_inc_dec(-1);
}

// helper function for 0x0B, 0x1B, 0x2B
uint8_t CPU::DEC_RR(uint8_t &reg1, uint8_t &reg2)
{
    uint16_t reg = (reg1 << 8) | reg2;
    reg -= 1;
    reg1 = (reg >> 8) & 0xFF;
    reg2 = reg & 0xFF;
    return 8;
}

// 0x0B
uint8_t CPU::DEC_BC()
{
    return DEC_RR(B, C);
}

// 0x1B
uint8_t CPU::DEC_DE()
{
    return DEC_RR(D, E);
}

// 0x2B
uint8_t CPU::DEC_HL()
{
    return DEC_RR(H, L);
}

// 0x3B
uint8_t CPU::DEC_SP()
{
    SP -= 1;
    PC += 1;
    return 8;
}

// helper function for 0x0C, 0x1C, 0x2C, 0x3C
uint8_t CPU::INC_R(uint8_t &reg)
{
    reg += 1;

    SET_FLAG_Z(CPU::F, reg == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (reg & 0x0F) == 0);

    return 4;
}

// 0x0C
uint8_t CPU::INC_C()
{
    return INC_R(C);
}

// 0x1C
uint8_t CPU::INC_E()
{
    return INC_R(E);
}

// 0x2C
uint8_t CPU::INC_L()
{
    return INC_R(L);
}

// 0x3C
uint8_t CPU::INC_A()
{
    return INC_R(A);
}

// 0x0D
uint8_t CPU::DEC_C()
{
    return DEC_R(C);
}

// 0x1D
uint8_t CPU::DEC_E()
{
    return DEC_R(E);
}

// 0x2D
uint8_t CPU::DEC_L()
{
    return DEC_R(L);
}

// 0x3D
uint8_t CPU::DEC_A()
{
    return DEC_R(A);
}

// 0x0E
uint8_t CPU::LD_C_d8()
{
    return LD_R_d8(C);
}

// 0x1E
uint8_t CPU::LD_E_d8()
{
    return LD_R_d8(E);
}

// 0x2E
uint8_t CPU::LD_L_d8()
{
    return LD_R_d8(L);
}

// 0x3E
uint8_t CPU::LD_A_d8()
{
    return LD_R_d8(A);
}

// 0x0F
uint8_t CPU::RRCA()
{
    uint8_t bit0 = A & 0x01;
    A = (A >> 1) | (bit0 << 7);

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, bit0 == 1);

    return 4;
}

// 0x1F
uint8_t CPU::RRA()
{
    uint8_t bit0 = A & 0x01;
    A = (A >> 1) | (GET_FLAG_C(CPU::F) << 7);

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, bit0 == 1);

    return 4;
}

// 0x2F
uint8_t CPU::CPL()
{
    A = ~A;

    SET_FLAG_N(CPU::F, true);
    SET_FLAG_H(CPU::F, true);

    return 4;
}

// 0x3F
uint8_t CPU::CCF()
{
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, !GET_FLAG_C(CPU::F));

    return 4;
}

// helper function for 0x40 - 0x7F (minus 0x76) and LD MEM_HL_R
uint8_t CPU::LD_R_R(uint8_t &reg1, uint8_t reg2)
{
    reg1 = reg2;
    return 4;
}

// 0x40
uint8_t CPU::LD_B_B()
{
    return LD_R_R(B, B);
}

// 0x41
uint8_t CPU::LD_B_C()
{
    return LD_R_R(B, C);
}

// 0x42
uint8_t CPU::LD_B_D()
{
    return LD_R_R(B, D);
}

// 0x43
uint8_t CPU::LD_B_E()
{
    return LD_R_R(B, E);
}

// 0x44
uint8_t CPU::LD_B_H()
{
    return LD_R_R(B, H);
}

// 0x45
uint8_t CPU::LD_B_L()
{
    return LD_R_R(B, L);
}

// 0x46
uint8_t CPU::LD_B_mem_HL()
{
    B = memory.readByte((H << 8) | L);
    return 8;
}

// 0x47
uint8_t CPU::LD_B_A()
{
    return LD_R_R(B, A);
}

// 0x48
uint8_t CPU::LD_C_B()
{
    return LD_R_R(C, B);
}

// 0x49
uint8_t CPU::LD_C_C()
{
    return LD_R_R(C, C);
}

// 0x4A
uint8_t CPU::LD_C_D()
{
    return LD_R_R(C, D);
}

// 0x4B
uint8_t CPU::LD_C_E()
{
    return LD_R_R(C, E);
}

// 0x4C
uint8_t CPU::LD_C_H()
{
    return LD_R_R(C, H);
}

// 0x4D
uint8_t CPU::LD_C_L()
{
    return LD_R_R(C, L);
}

// 0x4E
uint8_t CPU::LD_C_mem_HL()
{
    C = memory.readByte((H << 8) | L);
    return 8;
}

// 0x4F
uint8_t CPU::LD_C_A()
{
    return LD_R_R(C, A);
}

// 0x50
uint8_t CPU::LD_D_B()
{
    return LD_R_R(D, B);
}

// 0x51
uint8_t CPU::LD_D_C()
{
    return LD_R_R(D, C);
}

// 0x52
uint8_t CPU::LD_D_D()
{
    return LD_R_R(D, D);
}

// 0x53
uint8_t CPU::LD_D_E()
{
    return LD_R_R(D, E);
}

// 0x54
uint8_t CPU::LD_D_H()
{
    return LD_R_R(D, H);
}

// 0x55
uint8_t CPU::LD_D_L()
{
    return LD_R_R(D, L);
}

// 0x56
uint8_t CPU::LD_D_mem_HL()
{
    D = memory.readByte((H << 8) | L);
    return 8;
}

// 0x57
uint8_t CPU::LD_D_A()
{
    return LD_R_R(D, A);
}

// 0x58
uint8_t CPU::LD_E_B()
{
    return LD_R_R(E, B);
}

// 0x59
uint8_t CPU::LD_E_C()
{
    return LD_R_R(E, C);
}

// 0x5A
uint8_t CPU::LD_E_D()
{
    return LD_R_R(E, D);
}

// 0x5B
uint8_t CPU::LD_E_E()
{
    return LD_R_R(E, E);
}

// 0x5C
uint8_t CPU::LD_E_H()
{
    return LD_R_R(E, H);
}

// 0x5D
uint8_t CPU::LD_E_L()
{
    return LD_R_R(E, L);
}

// 0x5E
uint8_t CPU::LD_E_mem_HL()
{
    E = memory.readByte((H << 8) | L);
    return 8;
}

// 0x5F
uint8_t CPU::LD_E_A()
{
    return LD_R_R(E, A);
}

// 0x60
uint8_t CPU::LD_H_B()
{
    return LD_R_R(H, B);
}

// 0x61
uint8_t CPU::LD_H_C()
{
    return LD_R_R(H, C);
}

// 0x62
uint8_t CPU::LD_H_D()
{
    return LD_R_R(H, D);
}

// 0x63
uint8_t CPU::LD_H_E()
{
    return LD_R_R(H, E);
}

// 0x64
uint8_t CPU::LD_H_H()
{
    return LD_R_R(H, H);
}

// 0x65
uint8_t CPU::LD_H_L()
{
    return LD_R_R(H, L);
}

// 0x66
uint8_t CPU::LD_H_mem_HL()
{
    H = memory.readByte((H << 8) | L);
    return 8;
}

// 0x67
uint8_t CPU::LD_H_A()
{
    return LD_R_R(H, A);
}

// 0x68
uint8_t CPU::LD_L_B()
{
    return LD_R_R(L, B);
}

// 0x69
uint8_t CPU::LD_L_C()
{
    return LD_R_R(L, C);
}

// 0x6A
uint8_t CPU::LD_L_D()
{
    return LD_R_R(L, D);
}

// 0x6B
uint8_t CPU::LD_L_E()
{
    return LD_R_R(L, E);
}

// 0x6C
uint8_t CPU::LD_L_H()
{
    return LD_R_R(L, H);
}

// 0x6D
uint8_t CPU::LD_L_L()
{
    return LD_R_R(L, L);
}

// 0x6E
uint8_t CPU::LD_L_mem_HL()
{
    L = memory.readByte((H << 8) | L);
    return 8;
}

// 0x6F
uint8_t CPU::LD_L_A()
{
    return LD_R_R(L, A);
}

// 0x70
uint8_t CPU::LD_mem_HL_B()
{
    memory.writeByte((H << 8) | L, B);
    return 8;
}

// 0x71
uint8_t CPU::LD_mem_HL_C()
{
    memory.writeByte((H << 8) | L, C);
    return 8;
}

// 0x72
uint8_t CPU::LD_mem_HL_D()
{
    memory.writeByte((H << 8) | L, D);
    return 8;
}

// 0x73
uint8_t CPU::LD_mem_HL_E()
{
    memory.writeByte((H << 8) | L, E);
    return 8;
}

// 0x74
uint8_t CPU::LD_mem_HL_H()
{
    memory.writeByte((H << 8) | L, H);
    return 8;
}

// 0x75
uint8_t CPU::LD_mem_HL_L()
{
    memory.writeByte((H << 8) | L, L);
    return 8;
}

// 0x76
uint8_t CPU::HALT()
{
    // Check for halt bug condition (IME disabled and any interrupt requested)
    if (!IME && (memory.readByte(0x0F) & memory.readByte(0xFF) & 0x1F))
    {
        // Halt bug: Do not actually halt and do not increment PC
        PC--;
        printf("Halt bug triggered\n");
    }
    else
    {
        // Normal halt behavior
        halted = true;
    }
    return 4;
}

// 0x77
uint8_t CPU::LD_mem_HL_A()
{
    memory.writeByte((H << 8) | L, A);
    return 8;
}

// 0x78
uint8_t CPU::LD_A_B()
{
    return LD_R_R(A, B);
}

// 0x79
uint8_t CPU::LD_A_C()
{
    return LD_R_R(A, C);
}

// 0x7A
uint8_t CPU::LD_A_D()
{
    return LD_R_R(A, D);
}

// 0x7B
uint8_t CPU::LD_A_E()
{
    return LD_R_R(A, E);
}

// 0x7C
uint8_t CPU::LD_A_H()
{
    return LD_R_R(A, H);
}

// 0x7D
uint8_t CPU::LD_A_L()
{
    return LD_R_R(A, L);
}

// 0x7E
uint8_t CPU::LD_A_mem_HL()
{
    A = memory.readByte((H << 8) | L);
    return 8;
}

// 0x7F
uint8_t CPU::LD_A_A()
{
    return LD_R_R(A, A);
}

// helper function for 0x80 - 0x87 (minus 0x86)
uint8_t CPU::ADD_A_R(uint8_t reg)
{
    uint8_t result = A + reg;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (A & 0x0F) + (reg & 0x0F) > 0x0F);
    SET_FLAG_C(CPU::F, static_cast<uint16_t>(A) + reg > 0xFF);

    A = result;

    return 4;
}

// 0x80
uint8_t CPU::ADD_A_B()
{
    return ADD_A_R(B);
}

// 0x81
uint8_t CPU::ADD_A_C()
{
    return ADD_A_R(C);
}

// 0x82
uint8_t CPU::ADD_A_D()
{
    return ADD_A_R(D);
}

// 0x83
uint8_t CPU::ADD_A_E()
{
    return ADD_A_R(E);
}

// 0x84
uint8_t CPU::ADD_A_H()
{
    return ADD_A_R(H);
}

// 0x85
uint8_t CPU::ADD_A_L()
{
    return ADD_A_R(L);
}

// 0x86
uint8_t CPU::ADD_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return ADD_A_R(value);
}

// 0x87
uint8_t CPU::ADD_A_A()
{
    return ADD_A_R(A);
}

// helper function for 0x88 - 0x8F (minus 0x8E)
uint8_t CPU::ADC_A_R(uint8_t reg)
{
    uint8_t result = A + reg + GET_FLAG_C(CPU::F);

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, (A & 0x0F) + (reg & 0x0F) + GET_FLAG_C(CPU::F) > 0x0F);
    SET_FLAG_C(CPU::F, (static_cast<uint16_t>(A) + reg + GET_FLAG_C(CPU::F)) > 0xFF);

    A = result;

    return 4;
}

// 0x88
uint8_t CPU::ADC_A_B()
{
    return ADC_A_R(B);
}

// 0x89
uint8_t CPU::ADC_A_C()
{
    return ADC_A_R(C);
}

// 0x8A
uint8_t CPU::ADC_A_D()
{
    return ADC_A_R(D);
}

// 0x8B
uint8_t CPU::ADC_A_E()
{
    return ADC_A_R(E);
}

// 0x8C
uint8_t CPU::ADC_A_H()
{
    return ADC_A_R(H);
}

// 0x8D
uint8_t CPU::ADC_A_L()
{
    return ADC_A_R(L);
}

// 0x8E
uint8_t CPU::ADC_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return ADC_A_R(value);
}

// 0x8F
uint8_t CPU::ADC_A_A()
{
    return ADC_A_R(A);
}

// helper function for 0x90 - 0x97 (minus 0x96)
uint8_t CPU::SUB_A_R(uint8_t reg)
{
    uint8_t result = A - reg;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, true);
    SET_FLAG_H(CPU::F, (A & 0x0F) < (reg & 0x0F));
    SET_FLAG_C(CPU::F, A < reg);

    A = result;

    return 4;
}

// 0x90
uint8_t CPU::SUB_A_B()
{
    return SUB_A_R(B);
}

// 0x91
uint8_t CPU::SUB_A_C()
{
    return SUB_A_R(C);
}

// 0x92
uint8_t CPU::SUB_A_D()
{
    return SUB_A_R(D);
}

// 0x93
uint8_t CPU::SUB_A_E()
{
    return SUB_A_R(E);
}

// 0x94
uint8_t CPU::SUB_A_H()
{
    return SUB_A_R(H);
}

// 0x95
uint8_t CPU::SUB_A_L()
{
    return SUB_A_R(L);
}

// 0x96
uint8_t CPU::SUB_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return SUB_A_R(value);
}

// 0x97
uint8_t CPU::SUB_A_A()
{
    return SUB_A_R(A);
}

// helper function for 0x98 - 0x9F (minus 0x9E)
uint8_t CPU::SBC_A_R(uint8_t reg)
{
    uint8_t carry = GET_FLAG_C(CPU::F);
    uint8_t result = A - reg - carry;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, true);
    SET_FLAG_H(CPU::F, (A & 0x0F) < (reg & 0x0F) + carry);
    SET_FLAG_C(CPU::F, static_cast<uint16_t>(A) < static_cast<uint16_t>(reg) + carry);

    A = result;

    return 4;
}

// 0x98
uint8_t CPU::SBC_A_B()
{
    return SBC_A_R(B);
}

// 0x99
uint8_t CPU::SBC_A_C()
{
    return SBC_A_R(C);
}

// 0x9A
uint8_t CPU::SBC_A_D()
{
    return SBC_A_R(D);
}

// 0x9B
uint8_t CPU::SBC_A_E()
{
    return SBC_A_R(E);
}

// 0x9C
uint8_t CPU::SBC_A_H()
{
    return SBC_A_R(H);
}

// 0x9D
uint8_t CPU::SBC_A_L()
{
    return SBC_A_R(L);
}

// 0x9E
uint8_t CPU::SBC_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return SBC_A_R(value);
}

// 0x9F
uint8_t CPU::SBC_A_A()
{
    return SBC_A_R(A);
}

// helper function for 0xA0 - 0xA7 (minus 0xA6)
uint8_t CPU::AND_A_R(uint8_t reg)
{
    A &= reg;

    SET_FLAG_Z(CPU::F, A == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, true);
    SET_FLAG_C(CPU::F, false);

    return 4;
}

// 0xA0
uint8_t CPU::AND_A_B()
{
    return AND_A_R(B);
}

// 0xA1
uint8_t CPU::AND_A_C()
{
    return AND_A_R(C);
}

// 0xA2
uint8_t CPU::AND_A_D()
{
    return AND_A_R(D);
}

// 0xA3
uint8_t CPU::AND_A_E()
{
    return AND_A_R(E);
}

// 0xA4
uint8_t CPU::AND_A_H()
{
    return AND_A_R(H);
}

// 0xA5
uint8_t CPU::AND_A_L()
{
    return AND_A_R(L);
}

// 0xA6
uint8_t CPU::AND_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);

    return AND_A_R(value);
}

// 0xA7
uint8_t CPU::AND_A_A()
{
    return AND_A_R(A);
}

// helper function for 0xA8 - 0xAF (minus 0xAE)
uint8_t CPU::XOR_A_R(uint8_t reg)
{
    A ^= reg;

    SET_FLAG_Z(CPU::F, A == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, false);

    return 4;
}

// 0xA8
uint8_t CPU::XOR_A_B()
{
    return XOR_A_R(B);
}

// 0xA9
uint8_t CPU::XOR_A_C()
{
    return XOR_A_R(C);
}

// 0xAA
uint8_t CPU::XOR_A_D()
{
    return XOR_A_R(D);
}

// 0xAB
uint8_t CPU::XOR_A_E()
{
    return XOR_A_R(E);
}

// 0xAC
uint8_t CPU::XOR_A_H()
{
    return XOR_A_R(H);
}

// 0xAD
uint8_t CPU::XOR_A_L()
{
    return XOR_A_R(L);
}

// 0xAE
uint8_t CPU::XOR_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return XOR_A_R(value);
}

// 0xAF
uint8_t CPU::XOR_A_A()
{
    return XOR_A_R(A);
}

// helper function for 0xB0 - 0xB7 (minus 0xB6)
uint8_t CPU::OR_A_R(uint8_t reg)
{
    A |= reg;

    SET_FLAG_Z(CPU::F, A == 0);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, false);
    SET_FLAG_C(CPU::F, false);

    return 4;
}

// 0xB0
uint8_t CPU::OR_A_B()
{
    return OR_A_R(B);
}

// 0xB1
uint8_t CPU::OR_A_C()
{
    return OR_A_R(C);
}

// 0xB2
uint8_t CPU::OR_A_D()
{
    return OR_A_R(D);
}

// 0xB3
uint8_t CPU::OR_A_E()
{
    return OR_A_R(E);
}

// 0xB4
uint8_t CPU::OR_A_H()
{
    return OR_A_R(H);
}

// 0xB5
uint8_t CPU::OR_A_L()
{
    return OR_A_R(L);
}

// 0xB6
uint8_t CPU::OR_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return OR_A_R(value);
}

// 0xB7
uint8_t CPU::OR_A_A()
{
    return OR_A_R(A);
}

// helper function for 0xB8 - 0xBF (minus 0xBE)
uint8_t CPU::CP_A_R(uint8_t reg)
{
    uint8_t result = A - reg;

    SET_FLAG_Z(CPU::F, result == 0);
    SET_FLAG_N(CPU::F, true);
    SET_FLAG_H(CPU::F, (A & 0x0F) < (reg & 0x0F));
    SET_FLAG_C(CPU::F, A < reg);

    return 4;
}

// 0xB8
uint8_t CPU::CP_A_B()
{
    return CP_A_R(B);
}

// 0xB9
uint8_t CPU::CP_A_C()
{
    return CP_A_R(C);
}

// 0xBA
uint8_t CPU::CP_A_D()
{
    return CP_A_R(D);
}

// 0xBB
uint8_t CPU::CP_A_E()
{
    return CP_A_R(E);
}

// 0xBC
uint8_t CPU::CP_A_H()
{
    return CP_A_R(H);
}

// 0xBD
uint8_t CPU::CP_A_L()
{
    return CP_A_R(L);
}

// 0xBE
uint8_t CPU::CP_A_mem_HL()
{
    uint16_t HL = (H << 8) | L;
    uint8_t value = memory.readByte(HL);
    return CP_A_R(value);
}

// 0xBF
uint8_t CPU::CP_A_A()
{
    return CP_A_R(A);
}

uint8_t CPU::RET_cc(uint8_t flag)
{
    if (flag)
    {
        uint8_t low = memory.readByte(SP);
        SP += 1;

        uint8_t high = memory.readByte(SP);
        SP += 1;

        PC = (high << 8) | low;

        return 20;
    }
    else
    {
        return 8;
    }
}

// helper function for 0xE0
uint8_t CPU::LDH_mem_n_A(uint8_t offset)
{
    uint8_t address = fetchInstruction();
    memory.writeByte(0xFF00 + address, A);
    return 12;
}

// 0xC0
uint8_t CPU::RET_NZ()
{

    return RET_cc(!GET_FLAG_Z(CPU::F));
}

// 0xD0
uint8_t CPU::RET_NC()
{
    return RET_cc(!GET_FLAG_C(CPU::F));
}

// 0xE0
uint8_t CPU::LDH_mem_n_A()
{
    return LDH_mem_n_A(0x00);
}

// 0xF0
uint8_t CPU::LDH_A_mem_n()
{
    uint8_t address = fetchInstruction();
    A = memory.readByte(0xFF00 + address);
    return 12;
}

// helper function for 0xC1, 0xD1, 0xE1, 0xF1
uint8_t CPU::POP_RR(uint8_t &reg1, uint8_t &reg2)
{
    uint8_t low = memory.readByte(SP);
    SP += 1;
    uint8_t high = memory.readByte(SP);
    SP += 1;

    reg1 = high;
    reg2 = low;

    return 12;
}

// 0xC1
uint8_t CPU::POP_BC()
{
    return POP_RR(B, C);
}

// 0xD1
uint8_t CPU::POP_DE()
{
    return POP_RR(D, E);
}

// 0xE1
uint8_t CPU::POP_HL()
{
    return POP_RR(H, L);
}

// 0xF1
uint8_t CPU::POP_AF()
{
    uint8_t cycles = POP_RR(A, F);

    F &= 0xF0;

    return cycles;
}

// helper function for 0xC2, 0xD2
uint8_t CPU::JP_cc_nn(uint8_t flag)
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();

    if (flag)
    {
        PC = (high << 8) | low;
        return 16;
    }
    else
    {
        return 12;
    }
}

// 0xC2
uint8_t CPU::JP_NZ_nn()
{
    return JP_cc_nn(!GET_FLAG_Z(CPU::F));
}

// 0xD2
uint8_t CPU::JP_NC_nn()
{
    return JP_cc_nn(!GET_FLAG_C(CPU::F));
}

// 0xE2
uint8_t CPU::LD_mem_C_A()
{
    memory.writeByte(0xFF00 + C, A);
    return 8;
}

// 0xF2
uint8_t CPU::LDH_A_mem_C()
{
    A = memory.readByte(0xFF00 + C);
    return 8;
}

// 0xC3
uint8_t CPU::JP_nn()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    PC = (high << 8) | low;
    return 16;
}

// 0xF3
uint8_t CPU::DI()
{
    IME = false;
    return 4;
}

uint8_t CPU::CALL_cc_nn(uint8_t flag)
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    if (flag)
    {
        SP -= 1;
        memory.writeByte(SP, (PC >> 8) & 0xFF);
        SP -= 1;
        memory.writeByte(SP, PC & 0xFF);

        PC = (high << 8) | low;
        return 24;
    }
    else
    {
        return 12;
    }
}

// 0xC4
uint8_t CPU::CALL_NZ_nn()
{
    return CALL_cc_nn(!GET_FLAG_Z(CPU::F));
}

// 0xD4
uint8_t CPU::CALL_NC_nn()
{
    return CALL_cc_nn(!GET_FLAG_C(CPU::F));
}

// helper function for 0xC5, 0xD5, 0xE5, 0xF5
uint8_t CPU::PUSH_RR(uint8_t reg1, uint8_t reg2)
{
    SP -= 1;
    memory.writeByte(SP, reg1);
    SP -= 1;
    memory.writeByte(SP, reg2);
    return 16;
}

// 0xC5
uint8_t CPU::PUSH_BC()
{
    return PUSH_RR(B, C);
}

// 0xD5
uint8_t CPU::PUSH_DE()
{
    return PUSH_RR(D, E);
}

// 0xE5
uint8_t CPU::PUSH_HL()
{
    return PUSH_RR(H, L);
}

// 0xF5
uint8_t CPU::PUSH_AF()
{
    return PUSH_RR(A, F);
}

// 0xC6
uint8_t CPU::ADD_A_d8()
{
    uint8_t value = fetchInstruction();
    return ADD_A_R(value);
}

// 0xD6
uint8_t CPU::SUB_A_d8()
{
    uint8_t value = fetchInstruction();
    return SUB_A_R(value);
}

// 0xE6
uint8_t CPU::AND_A_d8()
{
    uint8_t value = fetchInstruction();
    return AND_A_R(value);
}

// 0xF6
uint8_t CPU::OR_A_d8()
{
    uint8_t value = fetchInstruction();
    return OR_A_R(value);
}

// helper function for 0xC7, 0xD7, 0xE7, 0xF7
uint8_t CPU::RST_n(uint_fast16_t offset)
{
    uint16_t returnAddress = PC;

    SP -= 1;
    memory.writeByte(SP, (returnAddress >> 8) & 0xFF);
    SP -= 1;
    memory.writeByte(SP, returnAddress & 0xFF);

    PC = offset;

    return 16;
}

// 0xC7
uint8_t CPU::RST_00H()
{
    return RST_n(0x00);
}

// 0xD7
uint8_t CPU::RST_10H()
{
    return RST_n(0x0010);
}

// 0xE7
uint8_t CPU::RST_20H()
{
    return RST_n(0x0020);
}

// 0xF7
uint8_t CPU::RST_30H()
{
    return RST_n(0x0030);
}

// 0xC8
uint8_t CPU::RET_Z()
{
    return RET_cc(GET_FLAG_Z(CPU::F));
}

// 0xD8
uint8_t CPU::RET_C()
{
    return RET_cc(GET_FLAG_C(CPU::F));
}

// 0xE8
uint8_t CPU::ADD_SP_r8()
{
    int8_t value = static_cast<int8_t>(fetchInstruction());
    int result = static_cast<int>(SP) + static_cast<int>(value);

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, ((SP ^ value ^ (result & 0xFFFF)) & 0x10) != 0);
    SET_FLAG_C(CPU::F, ((SP ^ value ^ (result & 0xFFFF)) & 0x100) != 0);

    SP = static_cast<uint16_t>(result);

    return 16;
}

// 0xF8
uint8_t CPU::LD_HL_SP_r8()
{
    int8_t value = static_cast<int8_t>(fetchInstruction());
    uint16_t result = SP + value;

    SET_FLAG_Z(CPU::F, false);
    SET_FLAG_N(CPU::F, false);
    SET_FLAG_H(CPU::F, ((SP ^ value ^ result) & 0x10) != 0);
    SET_FLAG_C(CPU::F, ((SP ^ value ^ result) & 0x100) != 0);

    H = (result >> 8) & 0xFF;
    L = result & 0xFF;

    return 12; // LD_HL_SP_r8 takes 12 cycles
}

// 0xC9
uint8_t CPU::RET()
{
    uint8_t low = memory.readByte(SP++);
    uint8_t high = memory.readByte(SP++);
    PC = (high << 8) | low;
    return 16;
}

// 0xD9
uint8_t CPU::RETI()
{
    uint8_t low = memory.readByte(SP++);
    uint8_t high = memory.readByte(SP++);

    PC = (high << 8) | low;
    IME = true;

    return 16;
}

// 0xE9
uint8_t CPU::JP_HL()
{
    PC = (H << 8) | L;
    return 4;
}

// 0xF9
uint8_t CPU::LD_SP_HL()
{
    SP = (H << 8) | L;
    return 8;
}

// 0xCA
uint8_t CPU::JP_Z_nn()
{
    return JP_cc_nn(GET_FLAG_Z(CPU::F));
}

// 0xDA
uint8_t CPU::JP_C_nn()
{
    return JP_cc_nn(GET_FLAG_C(CPU::F));
}

// 0xEA
uint8_t CPU::LD_mem_nn_A()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    uint16_t address = (high << 8) | low;
    memory.writeByte(address, A);
    return 16;
}

// 0xFA
uint8_t CPU::LD_A_mem_nn()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();
    uint16_t address = (high << 8) | low;
    A = memory.readByte(address);
    return 16;
}

// helper function for 0xCB
uint8_t CPU::CB()
{
    uint8_t opcode = fetchInstruction();
    uint8_t cycles = executeExtendedInstruction(opcode);

    return cycles;
}

// 0xFB
uint8_t CPU::EI()
{
    shouldEnableInterrupts = true;
    return 4;
}

// 0xCC
uint8_t CPU::CALL_Z_nn()
{
    return CALL_cc_nn(GET_FLAG_Z(CPU::F));
}

// 0xDC
uint8_t CPU::CALL_C_nn()
{
    return CALL_cc_nn(GET_FLAG_C(CPU::F));
}

// 0xCD
uint8_t CPU::CALL_nn()
{
    uint8_t low = fetchInstruction();
    uint8_t high = fetchInstruction();

    SP -= 1;
    memory.writeByte(SP, (PC >> 8) & 0xFF);
    SP -= 1;
    memory.writeByte(SP, PC & 0xFF);

    PC = (high << 8) | low;
    return 24;
}

// 0xCE
uint8_t CPU::ADC_A_d8()
{
    uint8_t value = fetchInstruction();
    return ADC_A_R(value);
}

// 0xDE
uint8_t CPU::SBC_A_d8()
{
    uint8_t value = fetchInstruction();
    return SBC_A_R(value);
}

// 0xEE
uint8_t CPU::XOR_A_d8()
{
    uint8_t value = fetchInstruction();
    return XOR_A_R(value);
}

// 0xFE
uint8_t CPU::CP_A_d8()
{
    uint8_t value = fetchInstruction();
    return CP_A_R(value);
}

// 0xCF
uint8_t CPU::RST_08H()
{
    return RST_n(0x0008);
}

// 0xDF
uint8_t CPU::RST_18H()
{
    return RST_n(0x0018);
}

// 0xEF
uint8_t CPU::RST_28H()
{
    return RST_n(0x0028);
}

// 0xFF
uint8_t CPU::RST_38H()
{
    return RST_n(0x0038);
}