#include "cpu.h"

typedef uint8_t (CPU::*InstructionFunc)();
InstructionFunc instructionTable[256];

// CB prefixed instructions
typedef uint8_t (CPU::*CBInstructionFunc)();
CBInstructionFunc CBInstructionTable[256];

bool CPU::stopped = false;
bool CPU::halted = false;
bool CPU::IME = false;
bool CPU::shouldEnableInterrupts = false;

uint8_t CPU::DIV = 0;
uint8_t CPU::TIMA = 0;
uint8_t CPU::TMA = 0;
uint8_t CPU::TAC = 0;

uint32_t CPU::divCounter = 0;
uint32_t CPU::timerCounter = 0;

FILE *status;

CPU::CPU(Memory &memory) : memory(memory)
{
#ifdef PLATFORM_ESP32
    fileReader.reset(new ESPFileReader());
#else
    fileReader.reset(new DesktopFileReader());
#endif

    CBInstructionTable[0x00] = &CPU::RLC_B;
    CBInstructionTable[0x01] = &CPU::RLC_C;
    CBInstructionTable[0x02] = &CPU::RLC_D;
    CBInstructionTable[0x03] = &CPU::RLC_E;
    CBInstructionTable[0x04] = &CPU::RLC_H;
    CBInstructionTable[0x05] = &CPU::RLC_L;
    CBInstructionTable[0x06] = &CPU::RLC_HL;
    CBInstructionTable[0x07] = &CPU::RLC_A;
    CBInstructionTable[0x08] = &CPU::RRC_B;
    CBInstructionTable[0x09] = &CPU::RRC_C;
    CBInstructionTable[0x0A] = &CPU::RRC_D;
    CBInstructionTable[0x0B] = &CPU::RRC_E;
    CBInstructionTable[0x0C] = &CPU::RRC_H;
    CBInstructionTable[0x0D] = &CPU::RRC_L;
    CBInstructionTable[0x0E] = &CPU::RRC_HL;
    CBInstructionTable[0x0F] = &CPU::RRC_A;
    CBInstructionTable[0x10] = &CPU::RL_B;
    CBInstructionTable[0x11] = &CPU::RL_C;
    CBInstructionTable[0x12] = &CPU::RL_D;
    CBInstructionTable[0x13] = &CPU::RL_E;
    CBInstructionTable[0x14] = &CPU::RL_H;
    CBInstructionTable[0x15] = &CPU::RL_L;
    CBInstructionTable[0x16] = &CPU::RL_HL;
    CBInstructionTable[0x17] = &CPU::RL_A;
    CBInstructionTable[0x18] = &CPU::RR_B;
    CBInstructionTable[0x19] = &CPU::RR_C;
    CBInstructionTable[0x1A] = &CPU::RR_D;
    CBInstructionTable[0x1B] = &CPU::RR_E;
    CBInstructionTable[0x1C] = &CPU::RR_H;
    CBInstructionTable[0x1D] = &CPU::RR_L;
    CBInstructionTable[0x1E] = &CPU::RR_HL;
    CBInstructionTable[0x1F] = &CPU::RR_A;
    CBInstructionTable[0x20] = &CPU::SLA_B;
    CBInstructionTable[0x21] = &CPU::SLA_C;
    CBInstructionTable[0x22] = &CPU::SLA_D;
    CBInstructionTable[0x23] = &CPU::SLA_E;
    CBInstructionTable[0x24] = &CPU::SLA_H;
    CBInstructionTable[0x25] = &CPU::SLA_L;
    CBInstructionTable[0x26] = &CPU::SLA_HL;
    CBInstructionTable[0x27] = &CPU::SLA_A;
    CBInstructionTable[0x28] = &CPU::SRA_B;
    CBInstructionTable[0x29] = &CPU::SRA_C;
    CBInstructionTable[0x2A] = &CPU::SRA_D;
    CBInstructionTable[0x2B] = &CPU::SRA_E;
    CBInstructionTable[0x2C] = &CPU::SRA_H;
    CBInstructionTable[0x2D] = &CPU::SRA_L;
    CBInstructionTable[0x2E] = &CPU::SRA_HL;
    CBInstructionTable[0x2F] = &CPU::SRA_A;
    CBInstructionTable[0x30] = &CPU::SWAP_B;
    CBInstructionTable[0x31] = &CPU::SWAP_C;
    CBInstructionTable[0x32] = &CPU::SWAP_D;
    CBInstructionTable[0x33] = &CPU::SWAP_E;
    CBInstructionTable[0x34] = &CPU::SWAP_H;
    CBInstructionTable[0x35] = &CPU::SWAP_L;
    CBInstructionTable[0x36] = &CPU::SWAP_HL;
    CBInstructionTable[0x37] = &CPU::SWAP_A;
    CBInstructionTable[0x38] = &CPU::SRL_B;
    CBInstructionTable[0x39] = &CPU::SRL_C;
    CBInstructionTable[0x3A] = &CPU::SRL_D;
    CBInstructionTable[0x3B] = &CPU::SRL_E;
    CBInstructionTable[0x3C] = &CPU::SRL_H;
    CBInstructionTable[0x3D] = &CPU::SRL_L;
    CBInstructionTable[0x3E] = &CPU::SRL_HL;
    CBInstructionTable[0x3F] = &CPU::SRL_A;
    CBInstructionTable[0x40] = &CPU::BIT_0_B;
    CBInstructionTable[0x41] = &CPU::BIT_0_C;
    CBInstructionTable[0x42] = &CPU::BIT_0_D;
    CBInstructionTable[0x43] = &CPU::BIT_0_E;
    CBInstructionTable[0x44] = &CPU::BIT_0_H;
    CBInstructionTable[0x45] = &CPU::BIT_0_L;
    CBInstructionTable[0x46] = &CPU::BIT_0_HL;
    CBInstructionTable[0x47] = &CPU::BIT_0_A;
    CBInstructionTable[0x48] = &CPU::BIT_1_B;
    CBInstructionTable[0x49] = &CPU::BIT_1_C;
    CBInstructionTable[0x4A] = &CPU::BIT_1_D;
    CBInstructionTable[0x4B] = &CPU::BIT_1_E;
    CBInstructionTable[0x4C] = &CPU::BIT_1_H;
    CBInstructionTable[0x4D] = &CPU::BIT_1_L;
    CBInstructionTable[0x4E] = &CPU::BIT_1_HL;
    CBInstructionTable[0x4F] = &CPU::BIT_1_A;
    CBInstructionTable[0x50] = &CPU::BIT_2_B;
    CBInstructionTable[0x51] = &CPU::BIT_2_C;
    CBInstructionTable[0x52] = &CPU::BIT_2_D;
    CBInstructionTable[0x53] = &CPU::BIT_2_E;
    CBInstructionTable[0x54] = &CPU::BIT_2_H;
    CBInstructionTable[0x55] = &CPU::BIT_2_L;
    CBInstructionTable[0x56] = &CPU::BIT_2_HL;
    CBInstructionTable[0x57] = &CPU::BIT_2_A;
    CBInstructionTable[0x58] = &CPU::BIT_3_B;
    CBInstructionTable[0x59] = &CPU::BIT_3_C;
    CBInstructionTable[0x5A] = &CPU::BIT_3_D;
    CBInstructionTable[0x5B] = &CPU::BIT_3_E;
    CBInstructionTable[0x5C] = &CPU::BIT_3_H;
    CBInstructionTable[0x5D] = &CPU::BIT_3_L;
    CBInstructionTable[0x5E] = &CPU::BIT_3_HL;
    CBInstructionTable[0x5F] = &CPU::BIT_3_A;
    CBInstructionTable[0x60] = &CPU::BIT_4_B;
    CBInstructionTable[0x61] = &CPU::BIT_4_C;
    CBInstructionTable[0x62] = &CPU::BIT_4_D;
    CBInstructionTable[0x63] = &CPU::BIT_4_E;
    CBInstructionTable[0x64] = &CPU::BIT_4_H;
    CBInstructionTable[0x65] = &CPU::BIT_4_L;
    CBInstructionTable[0x66] = &CPU::BIT_4_HL;
    CBInstructionTable[0x67] = &CPU::BIT_4_A;
    CBInstructionTable[0x68] = &CPU::BIT_5_B;
    CBInstructionTable[0x69] = &CPU::BIT_5_C;
    CBInstructionTable[0x6A] = &CPU::BIT_5_D;
    CBInstructionTable[0x6B] = &CPU::BIT_5_E;
    CBInstructionTable[0x6C] = &CPU::BIT_5_H;
    CBInstructionTable[0x6D] = &CPU::BIT_5_L;
    CBInstructionTable[0x6E] = &CPU::BIT_5_HL;
    CBInstructionTable[0x6F] = &CPU::BIT_5_A;
    CBInstructionTable[0x70] = &CPU::BIT_6_B;
    CBInstructionTable[0x71] = &CPU::BIT_6_C;
    CBInstructionTable[0x72] = &CPU::BIT_6_D;
    CBInstructionTable[0x73] = &CPU::BIT_6_E;
    CBInstructionTable[0x74] = &CPU::BIT_6_H;
    CBInstructionTable[0x75] = &CPU::BIT_6_L;
    CBInstructionTable[0x76] = &CPU::BIT_6_HL;
    CBInstructionTable[0x77] = &CPU::BIT_6_A;
    CBInstructionTable[0x78] = &CPU::BIT_7_B;
    CBInstructionTable[0x79] = &CPU::BIT_7_C;
    CBInstructionTable[0x7A] = &CPU::BIT_7_D;
    CBInstructionTable[0x7B] = &CPU::BIT_7_E;
    CBInstructionTable[0x7C] = &CPU::BIT_7_H;
    CBInstructionTable[0x7D] = &CPU::BIT_7_L;
    CBInstructionTable[0x7E] = &CPU::BIT_7_HL;
    CBInstructionTable[0x7F] = &CPU::BIT_7_A;
    CBInstructionTable[0x80] = &CPU::RES_0_B;
    CBInstructionTable[0x81] = &CPU::RES_0_C;
    CBInstructionTable[0x82] = &CPU::RES_0_D;
    CBInstructionTable[0x83] = &CPU::RES_0_E;
    CBInstructionTable[0x84] = &CPU::RES_0_H;
    CBInstructionTable[0x85] = &CPU::RES_0_L;
    CBInstructionTable[0x86] = &CPU::RES_0_HL;
    CBInstructionTable[0x87] = &CPU::RES_0_A;
    CBInstructionTable[0x88] = &CPU::RES_1_B;
    CBInstructionTable[0x89] = &CPU::RES_1_C;
    CBInstructionTable[0x8A] = &CPU::RES_1_D;
    CBInstructionTable[0x8B] = &CPU::RES_1_E;
    CBInstructionTable[0x8C] = &CPU::RES_1_H;
    CBInstructionTable[0x8D] = &CPU::RES_1_L;
    CBInstructionTable[0x8E] = &CPU::RES_1_HL;
    CBInstructionTable[0x8F] = &CPU::RES_1_A;
    CBInstructionTable[0x90] = &CPU::RES_2_B;
    CBInstructionTable[0x91] = &CPU::RES_2_C;
    CBInstructionTable[0x92] = &CPU::RES_2_D;
    CBInstructionTable[0x93] = &CPU::RES_2_E;
    CBInstructionTable[0x94] = &CPU::RES_2_H;
    CBInstructionTable[0x95] = &CPU::RES_2_L;
    CBInstructionTable[0x96] = &CPU::RES_2_HL;
    CBInstructionTable[0x97] = &CPU::RES_2_A;
    CBInstructionTable[0x98] = &CPU::RES_3_B;
    CBInstructionTable[0x99] = &CPU::RES_3_C;
    CBInstructionTable[0x9A] = &CPU::RES_3_D;
    CBInstructionTable[0x9B] = &CPU::RES_3_E;
    CBInstructionTable[0x9C] = &CPU::RES_3_H;
    CBInstructionTable[0x9D] = &CPU::RES_3_L;
    CBInstructionTable[0x9E] = &CPU::RES_3_HL;
    CBInstructionTable[0x9F] = &CPU::RES_3_A;
    CBInstructionTable[0xA0] = &CPU::RES_4_B;
    CBInstructionTable[0xA1] = &CPU::RES_4_C;
    CBInstructionTable[0xA2] = &CPU::RES_4_D;
    CBInstructionTable[0xA3] = &CPU::RES_4_E;
    CBInstructionTable[0xA4] = &CPU::RES_4_H;
    CBInstructionTable[0xA5] = &CPU::RES_4_L;
    CBInstructionTable[0xA6] = &CPU::RES_4_HL;
    CBInstructionTable[0xA7] = &CPU::RES_4_A;
    CBInstructionTable[0xA8] = &CPU::RES_5_B;
    CBInstructionTable[0xA9] = &CPU::RES_5_C;
    CBInstructionTable[0xAA] = &CPU::RES_5_D;
    CBInstructionTable[0xAB] = &CPU::RES_5_E;
    CBInstructionTable[0xAC] = &CPU::RES_5_H;
    CBInstructionTable[0xAD] = &CPU::RES_5_L;
    CBInstructionTable[0xAE] = &CPU::RES_5_HL;
    CBInstructionTable[0xAF] = &CPU::RES_5_A;
    CBInstructionTable[0xB0] = &CPU::RES_6_B;
    CBInstructionTable[0xB1] = &CPU::RES_6_C;
    CBInstructionTable[0xB2] = &CPU::RES_6_D;
    CBInstructionTable[0xB3] = &CPU::RES_6_E;
    CBInstructionTable[0xB4] = &CPU::RES_6_H;
    CBInstructionTable[0xB5] = &CPU::RES_6_L;
    CBInstructionTable[0xB6] = &CPU::RES_6_HL;
    CBInstructionTable[0xB7] = &CPU::RES_6_A;
    CBInstructionTable[0xB8] = &CPU::RES_7_B;
    CBInstructionTable[0xB9] = &CPU::RES_7_C;
    CBInstructionTable[0xBA] = &CPU::RES_7_D;
    CBInstructionTable[0xBB] = &CPU::RES_7_E;
    CBInstructionTable[0xBC] = &CPU::RES_7_H;
    CBInstructionTable[0xBD] = &CPU::RES_7_L;
    CBInstructionTable[0xBE] = &CPU::RES_7_HL;
    CBInstructionTable[0xBF] = &CPU::RES_7_A;
    CBInstructionTable[0xC0] = &CPU::SET_0_B;
    CBInstructionTable[0xC1] = &CPU::SET_0_C;
    CBInstructionTable[0xC2] = &CPU::SET_0_D;
    CBInstructionTable[0xC3] = &CPU::SET_0_E;
    CBInstructionTable[0xC4] = &CPU::SET_0_H;
    CBInstructionTable[0xC5] = &CPU::SET_0_L;
    CBInstructionTable[0xC6] = &CPU::SET_0_HL;
    CBInstructionTable[0xC7] = &CPU::SET_0_A;
    CBInstructionTable[0xC8] = &CPU::SET_1_B;
    CBInstructionTable[0xC9] = &CPU::SET_1_C;
    CBInstructionTable[0xCA] = &CPU::SET_1_D;
    CBInstructionTable[0xCB] = &CPU::SET_1_E;
    CBInstructionTable[0xCC] = &CPU::SET_1_H;
    CBInstructionTable[0xCD] = &CPU::SET_1_L;
    CBInstructionTable[0xCE] = &CPU::SET_1_HL;
    CBInstructionTable[0xCF] = &CPU::SET_1_A;
    CBInstructionTable[0xD0] = &CPU::SET_2_B;
    CBInstructionTable[0xD1] = &CPU::SET_2_C;
    CBInstructionTable[0xD2] = &CPU::SET_2_D;
    CBInstructionTable[0xD3] = &CPU::SET_2_E;
    CBInstructionTable[0xD4] = &CPU::SET_2_H;
    CBInstructionTable[0xD5] = &CPU::SET_2_L;
    CBInstructionTable[0xD6] = &CPU::SET_2_HL;
    CBInstructionTable[0xD7] = &CPU::SET_2_A;
    CBInstructionTable[0xD8] = &CPU::SET_3_B;
    CBInstructionTable[0xD9] = &CPU::SET_3_C;
    CBInstructionTable[0xDA] = &CPU::SET_3_D;
    CBInstructionTable[0xDB] = &CPU::SET_3_E;
    CBInstructionTable[0xDC] = &CPU::SET_3_H;
    CBInstructionTable[0xDD] = &CPU::SET_3_L;
    CBInstructionTable[0xDE] = &CPU::SET_3_HL;
    CBInstructionTable[0xDF] = &CPU::SET_3_A;
    CBInstructionTable[0xE0] = &CPU::SET_4_B;
    CBInstructionTable[0xE1] = &CPU::SET_4_C;
    CBInstructionTable[0xE2] = &CPU::SET_4_D;
    CBInstructionTable[0xE3] = &CPU::SET_4_E;
    CBInstructionTable[0xE4] = &CPU::SET_4_H;
    CBInstructionTable[0xE5] = &CPU::SET_4_L;
    CBInstructionTable[0xE6] = &CPU::SET_4_HL;
    CBInstructionTable[0xE7] = &CPU::SET_4_A;
    CBInstructionTable[0xE8] = &CPU::SET_5_B;
    CBInstructionTable[0xE9] = &CPU::SET_5_C;
    CBInstructionTable[0xEA] = &CPU::SET_5_D;
    CBInstructionTable[0xEB] = &CPU::SET_5_E;
    CBInstructionTable[0xEC] = &CPU::SET_5_H;
    CBInstructionTable[0xED] = &CPU::SET_5_L;
    CBInstructionTable[0xEE] = &CPU::SET_5_HL;
    CBInstructionTable[0xEF] = &CPU::SET_5_A;
    CBInstructionTable[0xF0] = &CPU::SET_6_B;
    CBInstructionTable[0xF1] = &CPU::SET_6_C;
    CBInstructionTable[0xF2] = &CPU::SET_6_D;
    CBInstructionTable[0xF3] = &CPU::SET_6_E;
    CBInstructionTable[0xF4] = &CPU::SET_6_H;
    CBInstructionTable[0xF5] = &CPU::SET_6_L;
    CBInstructionTable[0xF6] = &CPU::SET_6_HL;
    CBInstructionTable[0xF7] = &CPU::SET_6_A;
    CBInstructionTable[0xF8] = &CPU::SET_7_B;
    CBInstructionTable[0xF9] = &CPU::SET_7_C;
    CBInstructionTable[0xFA] = &CPU::SET_7_D;
    CBInstructionTable[0xFB] = &CPU::SET_7_E;
    CBInstructionTable[0xFC] = &CPU::SET_7_H;
    CBInstructionTable[0xFD] = &CPU::SET_7_L;
    CBInstructionTable[0xFE] = &CPU::SET_7_HL;
    CBInstructionTable[0xFF] = &CPU::SET_7_A;

    instructionTable[0x00] = &CPU::NoOperation;
    instructionTable[0x10] = &CPU::Stop;
    instructionTable[0x20] = &CPU::JR_NZ_i8;
    instructionTable[0x30] = &CPU::JR_NC_i8;
    instructionTable[0x01] = &CPU::LD_BC_d16;
    instructionTable[0x11] = &CPU::LD_DE_d16;
    instructionTable[0x21] = &CPU::LD_HL_d16;
    instructionTable[0x31] = &CPU::LD_SP_d16;
    instructionTable[0x02] = &CPU::LD_mem_BC_A;
    instructionTable[0x12] = &CPU::LD_mem_DE_A;
    instructionTable[0x22] = &CPU::LD_mem_HL_inc_A;
    instructionTable[0x32] = &CPU::LD_mem_HL_dec_A;
    instructionTable[0x03] = &CPU::INC_BC;
    instructionTable[0x13] = &CPU::INC_DE;
    instructionTable[0x23] = &CPU::INC_HL;
    instructionTable[0x33] = &CPU::INC_SP;
    instructionTable[0x04] = &CPU::INC_B;
    instructionTable[0x14] = &CPU::INC_D;
    instructionTable[0x24] = &CPU::INC_H;
    instructionTable[0x34] = &CPU::INC_mem_HL;
    instructionTable[0x05] = &CPU::DEC_B;
    instructionTable[0x15] = &CPU::DEC_D;
    instructionTable[0x25] = &CPU::DEC_H;
    instructionTable[0x35] = &CPU::DEC_mem_HL;
    instructionTable[0x06] = &CPU::LD_B_d8;
    instructionTable[0x16] = &CPU::LD_D_d8;
    instructionTable[0x26] = &CPU::LD_H_d8;
    instructionTable[0x36] = &CPU::LD_mem_HL_d8;
    instructionTable[0x07] = &CPU::RLCA;
    instructionTable[0x17] = &CPU::RLA;
    instructionTable[0x27] = &CPU::DAA;
    instructionTable[0x37] = &CPU::SCF;
    instructionTable[0x08] = &CPU::LD_mem_d16_SP;
    instructionTable[0x18] = &CPU::JR_i8;
    instructionTable[0x28] = &CPU::JR_Z_i8;
    instructionTable[0x38] = &CPU::JR_C_i8;
    instructionTable[0x09] = &CPU::ADD_HL_BC;
    instructionTable[0x19] = &CPU::ADD_HL_DE;
    instructionTable[0x29] = &CPU::ADD_HL_HL;
    instructionTable[0x39] = &CPU::ADD_HL_SP;
    instructionTable[0x0A] = &CPU::LD_A_mem_BC;
    instructionTable[0x1A] = &CPU::LD_A_mem_DE;
    instructionTable[0x2A] = &CPU::LD_A_mem_HL_inc;
    instructionTable[0x3A] = &CPU::LD_A_mem_HL_dec;
    instructionTable[0x0B] = &CPU::DEC_BC;
    instructionTable[0x1B] = &CPU::DEC_DE;
    instructionTable[0x2B] = &CPU::DEC_HL;
    instructionTable[0x3B] = &CPU::DEC_SP;
    instructionTable[0x0C] = &CPU::INC_C;
    instructionTable[0x1C] = &CPU::INC_E;
    instructionTable[0x2C] = &CPU::INC_L;
    instructionTable[0x3C] = &CPU::INC_A;
    instructionTable[0x0D] = &CPU::DEC_C;
    instructionTable[0x1D] = &CPU::DEC_E;
    instructionTable[0x2D] = &CPU::DEC_L;
    instructionTable[0x3D] = &CPU::DEC_A;
    instructionTable[0x0E] = &CPU::LD_C_d8;
    instructionTable[0x1E] = &CPU::LD_E_d8;
    instructionTable[0x2E] = &CPU::LD_L_d8;
    instructionTable[0x3E] = &CPU::LD_A_d8;
    instructionTable[0x0F] = &CPU::RRCA;
    instructionTable[0x1F] = &CPU::RRA;
    instructionTable[0x2F] = &CPU::CPL;
    instructionTable[0x3F] = &CPU::CCF;
    instructionTable[0x40] = &CPU::LD_B_B;
    instructionTable[0x41] = &CPU::LD_B_C;
    instructionTable[0x42] = &CPU::LD_B_D;
    instructionTable[0x43] = &CPU::LD_B_E;
    instructionTable[0x44] = &CPU::LD_B_H;
    instructionTable[0x45] = &CPU::LD_B_L;
    instructionTable[0x46] = &CPU::LD_B_mem_HL;
    instructionTable[0x47] = &CPU::LD_B_A;
    instructionTable[0x48] = &CPU::LD_C_B;
    instructionTable[0x49] = &CPU::LD_C_C;
    instructionTable[0x4A] = &CPU::LD_C_D;
    instructionTable[0x4B] = &CPU::LD_C_E;
    instructionTable[0x4C] = &CPU::LD_C_H;
    instructionTable[0x4D] = &CPU::LD_C_L;
    instructionTable[0x4E] = &CPU::LD_C_mem_HL;
    instructionTable[0x4F] = &CPU::LD_C_A;
    instructionTable[0x50] = &CPU::LD_D_B;
    instructionTable[0x51] = &CPU::LD_D_C;
    instructionTable[0x52] = &CPU::LD_D_D;
    instructionTable[0x53] = &CPU::LD_D_E;
    instructionTable[0x54] = &CPU::LD_D_H;
    instructionTable[0x55] = &CPU::LD_D_L;
    instructionTable[0x56] = &CPU::LD_D_mem_HL;
    instructionTable[0x57] = &CPU::LD_D_A;
    instructionTable[0x58] = &CPU::LD_E_B;
    instructionTable[0x59] = &CPU::LD_E_C;
    instructionTable[0x5A] = &CPU::LD_E_D;
    instructionTable[0x5B] = &CPU::LD_E_E;
    instructionTable[0x5C] = &CPU::LD_E_H;
    instructionTable[0x5D] = &CPU::LD_E_L;
    instructionTable[0x5E] = &CPU::LD_E_mem_HL;
    instructionTable[0x5F] = &CPU::LD_E_A;
    instructionTable[0x60] = &CPU::LD_H_B;
    instructionTable[0x61] = &CPU::LD_H_C;
    instructionTable[0x62] = &CPU::LD_H_D;
    instructionTable[0x63] = &CPU::LD_H_E;
    instructionTable[0x64] = &CPU::LD_H_H;
    instructionTable[0x65] = &CPU::LD_H_L;
    instructionTable[0x66] = &CPU::LD_H_mem_HL;
    instructionTable[0x67] = &CPU::LD_H_A;
    instructionTable[0x68] = &CPU::LD_L_B;
    instructionTable[0x69] = &CPU::LD_L_C;
    instructionTable[0x6A] = &CPU::LD_L_D;
    instructionTable[0x6B] = &CPU::LD_L_E;
    instructionTable[0x6C] = &CPU::LD_L_H;
    instructionTable[0x6D] = &CPU::LD_L_L;
    instructionTable[0x6E] = &CPU::LD_L_mem_HL;
    instructionTable[0x6F] = &CPU::LD_L_A;
    instructionTable[0x70] = &CPU::LD_mem_HL_B;
    instructionTable[0x71] = &CPU::LD_mem_HL_C;
    instructionTable[0x72] = &CPU::LD_mem_HL_D;
    instructionTable[0x73] = &CPU::LD_mem_HL_E;
    instructionTable[0x74] = &CPU::LD_mem_HL_H;
    instructionTable[0x75] = &CPU::LD_mem_HL_L;
    instructionTable[0x76] = &CPU::HALT;
    instructionTable[0x77] = &CPU::LD_mem_HL_A;
    instructionTable[0x78] = &CPU::LD_A_B;
    instructionTable[0x79] = &CPU::LD_A_C;
    instructionTable[0x7A] = &CPU::LD_A_D;
    instructionTable[0x7B] = &CPU::LD_A_E;
    instructionTable[0x7C] = &CPU::LD_A_H;
    instructionTable[0x7D] = &CPU::LD_A_L;
    instructionTable[0x7E] = &CPU::LD_A_mem_HL;
    instructionTable[0x7F] = &CPU::LD_A_A;
    instructionTable[0x80] = &CPU::ADD_A_B;
    instructionTable[0x81] = &CPU::ADD_A_C;
    instructionTable[0x82] = &CPU::ADD_A_D;
    instructionTable[0x83] = &CPU::ADD_A_E;
    instructionTable[0x84] = &CPU::ADD_A_H;
    instructionTable[0x85] = &CPU::ADD_A_L;
    instructionTable[0x86] = &CPU::ADD_A_mem_HL;
    instructionTable[0x87] = &CPU::ADD_A_A;
    instructionTable[0x88] = &CPU::ADC_A_B;
    instructionTable[0x89] = &CPU::ADC_A_C;
    instructionTable[0x8A] = &CPU::ADC_A_D;
    instructionTable[0x8B] = &CPU::ADC_A_E;
    instructionTable[0x8C] = &CPU::ADC_A_H;
    instructionTable[0x8D] = &CPU::ADC_A_L;
    instructionTable[0x8E] = &CPU::ADC_A_mem_HL;
    instructionTable[0x8F] = &CPU::ADC_A_A;
    instructionTable[0x90] = &CPU::SUB_A_B;
    instructionTable[0x91] = &CPU::SUB_A_C;
    instructionTable[0x92] = &CPU::SUB_A_D;
    instructionTable[0x93] = &CPU::SUB_A_E;
    instructionTable[0x94] = &CPU::SUB_A_H;
    instructionTable[0x95] = &CPU::SUB_A_L;
    instructionTable[0x96] = &CPU::SUB_A_mem_HL;
    instructionTable[0x97] = &CPU::SUB_A_A;
    instructionTable[0x98] = &CPU::SBC_A_B;
    instructionTable[0x99] = &CPU::SBC_A_C;
    instructionTable[0x9A] = &CPU::SBC_A_D;
    instructionTable[0x9B] = &CPU::SBC_A_E;
    instructionTable[0x9C] = &CPU::SBC_A_H;
    instructionTable[0x9D] = &CPU::SBC_A_L;
    instructionTable[0x9E] = &CPU::SBC_A_mem_HL;
    instructionTable[0x9F] = &CPU::SBC_A_A;
    instructionTable[0xA0] = &CPU::AND_A_B;
    instructionTable[0xA1] = &CPU::AND_A_C;
    instructionTable[0xA2] = &CPU::AND_A_D;
    instructionTable[0xA3] = &CPU::AND_A_E;
    instructionTable[0xA4] = &CPU::AND_A_H;
    instructionTable[0xA5] = &CPU::AND_A_L;
    instructionTable[0xA6] = &CPU::AND_A_mem_HL;
    instructionTable[0xA7] = &CPU::AND_A_A;
    instructionTable[0xA8] = &CPU::XOR_A_B;
    instructionTable[0xA9] = &CPU::XOR_A_C;
    instructionTable[0xAA] = &CPU::XOR_A_D;
    instructionTable[0xAB] = &CPU::XOR_A_E;
    instructionTable[0xAC] = &CPU::XOR_A_H;
    instructionTable[0xAD] = &CPU::XOR_A_L;
    instructionTable[0xAE] = &CPU::XOR_A_mem_HL;
    instructionTable[0xAF] = &CPU::XOR_A_A;
    instructionTable[0xB0] = &CPU::OR_A_B;
    instructionTable[0xB1] = &CPU::OR_A_C;
    instructionTable[0xB2] = &CPU::OR_A_D;
    instructionTable[0xB3] = &CPU::OR_A_E;
    instructionTable[0xB4] = &CPU::OR_A_H;
    instructionTable[0xB5] = &CPU::OR_A_L;
    instructionTable[0xB6] = &CPU::OR_A_mem_HL;
    instructionTable[0xB7] = &CPU::OR_A_A;
    instructionTable[0xB8] = &CPU::CP_A_B;
    instructionTable[0xB9] = &CPU::CP_A_C;
    instructionTable[0xBA] = &CPU::CP_A_D;
    instructionTable[0xBB] = &CPU::CP_A_E;
    instructionTable[0xBC] = &CPU::CP_A_H;
    instructionTable[0xBD] = &CPU::CP_A_L;
    instructionTable[0xBE] = &CPU::CP_A_mem_HL;
    instructionTable[0xBF] = &CPU::CP_A_A;
    instructionTable[0xC0] = &CPU::RET_NZ;
    instructionTable[0xD0] = &CPU::RET_NC;
    instructionTable[0xE0] = &CPU::LDH_mem_n_A;
    instructionTable[0xF0] = &CPU::LDH_A_mem_n;
    instructionTable[0xC1] = &CPU::POP_BC;
    instructionTable[0xD1] = &CPU::POP_DE;
    instructionTable[0xE1] = &CPU::POP_HL;
    instructionTable[0xF1] = &CPU::POP_AF;
    instructionTable[0xC2] = &CPU::JP_NZ_nn;
    instructionTable[0xD2] = &CPU::JP_NC_nn;
    instructionTable[0xE2] = &CPU::LD_mem_C_A;
    instructionTable[0xF2] = &CPU::LDH_A_mem_C;
    instructionTable[0xC3] = &CPU::JP_nn;
    instructionTable[0xD3] = &CPU::INVALID;
    instructionTable[0xE3] = &CPU::INVALID;
    instructionTable[0xF3] = &CPU::DI;
    instructionTable[0xC4] = &CPU::CALL_NZ_nn;
    instructionTable[0xD4] = &CPU::CALL_NC_nn;
    instructionTable[0xE4] = &CPU::INVALID;
    instructionTable[0xF4] = &CPU::INVALID;
    instructionTable[0xC5] = &CPU::PUSH_BC;
    instructionTable[0xD5] = &CPU::PUSH_DE;
    instructionTable[0xE5] = &CPU::PUSH_HL;
    instructionTable[0xF5] = &CPU::PUSH_AF;
    instructionTable[0xC6] = &CPU::ADD_A_d8;
    instructionTable[0xD6] = &CPU::SUB_A_d8;
    instructionTable[0xE6] = &CPU::AND_A_d8;
    instructionTable[0xF6] = &CPU::OR_A_d8;
    instructionTable[0xC7] = &CPU::RST_00H;
    instructionTable[0xD7] = &CPU::RST_10H;
    instructionTable[0xE7] = &CPU::RST_20H;
    instructionTable[0xF7] = &CPU::RST_30H;
    instructionTable[0xC8] = &CPU::RET_Z;
    instructionTable[0xD8] = &CPU::RET_C;
    instructionTable[0xE8] = &CPU::ADD_SP_r8;
    instructionTable[0xF8] = &CPU::LD_HL_SP_r8;
    instructionTable[0xC9] = &CPU::RET;
    instructionTable[0xD9] = &CPU::RETI;
    instructionTable[0xE9] = &CPU::JP_HL;
    instructionTable[0xF9] = &CPU::LD_SP_HL;
    instructionTable[0xCA] = &CPU::JP_Z_nn;
    instructionTable[0xDA] = &CPU::JP_C_nn;
    instructionTable[0xEA] = &CPU::LD_mem_nn_A;
    instructionTable[0xFA] = &CPU::LD_A_mem_nn;
    instructionTable[0xCB] = &CPU::CB;
    instructionTable[0xDB] = &CPU::INVALID;
    instructionTable[0xEB] = &CPU::INVALID;
    instructionTable[0xFB] = &CPU::EI;
    instructionTable[0xCC] = &CPU::CALL_Z_nn;
    instructionTable[0xDC] = &CPU::CALL_C_nn;
    instructionTable[0xEC] = &CPU::INVALID;
    instructionTable[0xFC] = &CPU::INVALID;
    instructionTable[0xCD] = &CPU::CALL_nn;
    instructionTable[0xDD] = &CPU::INVALID;
    instructionTable[0xED] = &CPU::INVALID;
    instructionTable[0xFD] = &CPU::INVALID;
    instructionTable[0xCE] = &CPU::ADC_A_d8;
    instructionTable[0xDE] = &CPU::SBC_A_d8;
    instructionTable[0xEE] = &CPU::XOR_A_d8;
    instructionTable[0xFE] = &CPU::CP_A_d8;
    instructionTable[0xCF] = &CPU::RST_08H;
    instructionTable[0xDF] = &CPU::RST_18H;
    instructionTable[0xEF] = &CPU::RST_28H;
    instructionTable[0xFF] = &CPU::RST_38H;
}

CPU::~CPU()
{
    // Destructor implementation
}

void CPU::reset()
{
    // Reset implementation

    // Set the program counter to the start of the ROM
    PC = 0x100;

    // Set the stack pointer to the start of the stack
    SP = 0xFFFE;

    // Set the registers to their initial values
    A = 0x01;
    F = 0xB0;
    B = 0x00;
    C = 0x13;
    D = 0x00;
    E = 0xD8;
    H = 0x01;
    L = 0x4D;

    status = fopen("status.txt", "w");
}

uint8_t CPU::fetchInstruction()
{
    // Fetch the next instruction from memory
    Logger logger;
    uint8_t opcode = memory.readByte(PC);
    // logger.println(logger.formatLogMessage(A, F, B, C, D, E, H, L, SP, PC, PC, PC + 1, PC + 2, PC + 3));
    debugCounter++;

    return opcode;
}

uint8_t CPU::executeExtendedInstruction(uint8_t opcode)
{
    // std::cout << "Executing extended instruction: " << std::hex << static_cast<int>(opcode) << std::endl;
    InstructionFunc func = CBInstructionTable[opcode];
    uint8_t cycles = (this->*func)();
    return cycles;
}

uint8_t CPU::executeCycle()
{
    uint8_t cycles = 0;
    // Handle interrupts if IME is set and there's a pending interrupt
    handleInterrupts();

    // Fetch and execute instruction if not halted or stopped
    if (!halted && !stopped)
    {
        uint8_t opcode = fetchInstruction();
        cycles = executeInstruction(opcode);
        if (shouldEnableInterrupts)
        {
            IME = true;
            shouldEnableInterrupts = false;
        }
    }

    updateTimers(cycles);

    return cycles;
}

void CPU::handleInterrupts()
{
    if (!IME || (memory.readByte(0xFFFF) & memory.readByte(0xFF0F)) == 0)
    {
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        uint8_t mask = 1 << i;
        uint8_t interruptEnabled = memory.readByte(0xFFFF) & mask;
        uint8_t interruptRequested = memory.readByte(0xFF0F) & mask;

        // Check if both interrupt is enabled and requested
        if (interruptEnabled && interruptRequested)
        {
            // Disable further interrupts
            IME = false;

            // Clear the interrupt request flag
            memory.writeByte(0xFF0F, memory.readByte(0xFF0F) & ~mask);

            // Push current PC on stack
            memory.writeByte(--SP, PC >> 8);
            memory.writeByte(--SP, PC & 0xFF);

            // Jump to the interrupt handler
            switch (i)
            {
            case 0:
                PC = 0x0040; // V-Blank
                break;
            case 1:
                PC = 0x0048; // LCD STAT
                break;
            case 2:
                PC = 0x0050; // Timer
                break;
            case 3:
                PC = 0x0058; // Serial
                break;
            case 4:
                PC = 0x0060; // Joypad
                break;
            }
            return;
        }
    }
}

void CPU::logStatus()
{
    // print registers to file
    uint8_t mem0 = memory.readByte(PC);
    uint8_t mem1 = memory.readByte(PC + 1);
    uint8_t mem2 = memory.readByte(PC + 2);
    uint8_t mem3 = memory.readByte(PC + 3);

    fprintf(status, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
            A, F, B, C, D, E, H, L, SP, PC, mem0, mem1, mem2, mem3);
}

uint8_t CPU::executeInstruction(uint8_t opcode)
{
    logStatus();

    InstructionFunc func = instructionTable[opcode];
    return (this->*func)();
}

void CPU::updateTimers(uint16_t cycles)
{
    // Update divider register
    divCounter += cycles;
    if (divCounter >= 256)
    {
        memory.writeByte(0xFF04, memory.readByte(0xFF04) + 1);
        divCounter -= 256;
    }

    // Update timer register
    uint8_t timerControl = memory.readByte(0xFF07);
    if (timerControl & 0x04)
    {
        timerCounter += cycles;
        uint16_t timerFrequency = 1024;
        switch (timerControl & 0x03)
        {
        case 0:
            timerFrequency = 1024;
            break;
        case 1:
            timerFrequency = 16;
            break;
        case 2:
            timerFrequency = 64;
            break;
        case 3:
            timerFrequency = 256;
            break;
        }
        if (timerCounter >= timerFrequency)
        {
            uint8_t timerValue = memory.readByte(0xFF05);
            timerValue++;
            if (timerValue == 0)
            {
                memory.writeByte(0xFF05, memory.readByte(0xFF06));        // Reload TIMA with TMA
                memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x04); // Set Timer interrupt flag
            }
            else
            {
                memory.writeByte(0xFF05, timerValue);
            }
            timerCounter -= timerFrequency;
        }
    }
}