#define FLAG_Z_BIT 7
#define FLAG_N_BIT 6
#define FLAG_H_BIT 5
#define FLAG_C_BIT 4

#define SET_FLAG_Z(F, val) ((val) ? ((F) |= (1 << FLAG_Z_BIT)) : ((F) &= ~(1 << FLAG_Z_BIT)))
#define SET_FLAG_N(F, val) ((val) ? ((F) |= (1 << FLAG_N_BIT)) : ((F) &= ~(1 << FLAG_N_BIT)))
#define SET_FLAG_H(F, val) ((val) ? ((F) |= (1 << FLAG_H_BIT)) : ((F) &= ~(1 << FLAG_H_BIT)))
#define SET_FLAG_C(F, val) ((val) ? ((F) |= (1 << FLAG_C_BIT)) : ((F) &= ~(1 << FLAG_C_BIT)))

#define CLEAR_FLAG_Z(F) ((F) &= ~(1 << FLAG_Z_BIT))
#define CLEAR_FLAG_N(F) ((F) &= ~(1 << FLAG_N_BIT))
#define CLEAR_FLAG_H(F) ((F) &= ~(1 << FLAG_H_BIT))
#define CLEAR_FLAG_C(F) ((F) &= ~(1 << FLAG_C_BIT))

#define GET_FLAG_Z(F) (((F) >> FLAG_Z_BIT) & 1)
#define GET_FLAG_N(F) (((F) >> FLAG_N_BIT) & 1)
#define GET_FLAG_H(F) (((F) >> FLAG_H_BIT) & 1)
#define GET_FLAG_C(F) (((F) >> FLAG_C_BIT) & 1)

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "../memory/memory.h"
#include "../../logger/logger.h"
#include <string>
#include "../../file-reader/file-reader.h"

#ifdef PLATFORM_ESP32
#include "../../file-reader/esp-file-reader.h"
#else
#include "../../file-reader/desktop-file-reader.h"
#endif

class CPU
{
public:
    CPU(Memory &memory);
    ~CPU();

    void reset();
    uint8_t executeCycle();

    void handleInterrupts();
    void setFlag();
    bool checkFlag();
    void updateTimers(uint16_t cycles);
    uint8_t fetchInstruction();
    uint8_t executeExtendedInstruction(uint8_t opcode);
    uint32_t debugCounter = 0;

    static bool shouldEnableInterrupts;

    // Stop flag
    static bool stopped;

    // Halt flag
    static bool halted;

    // Interrupt master enable flag
    static bool IME;

    // Interrupt enable register
    static uint8_t IE;

    // Interrupt request register
    static uint8_t IF;

    // Timer registers
    static uint8_t DIV;
    static uint8_t TIMA;
    static uint8_t TMA;
    static uint8_t TAC;

    // Timer counters
    static uint32_t divCounter;
    static uint32_t timerCounter;

    // Opcode function declarations
    uint8_t INVALID();
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

    uint8_t RLCA();
    uint8_t RLA();
    uint8_t DAA();
    uint8_t SCF();

    uint8_t LD_mem_d16_SP();
    uint8_t JR_i8();
    uint8_t JR_Z_i8();
    uint8_t JR_C_i8();

    uint8_t ADD_HL_BC();
    uint8_t ADD_HL_DE();
    uint8_t ADD_HL_HL();
    uint8_t ADD_HL_SP();

    uint8_t LD_A_mem_BC();
    uint8_t LD_A_mem_DE();
    uint8_t LD_A_mem_HL_inc();
    uint8_t LD_A_mem_HL_dec();

    uint8_t DEC_BC();
    uint8_t DEC_DE();
    uint8_t DEC_HL();
    uint8_t DEC_SP();

    uint8_t INC_C();
    uint8_t INC_E();
    uint8_t INC_L();
    uint8_t INC_A();

    uint8_t DEC_C();
    uint8_t DEC_E();
    uint8_t DEC_L();
    uint8_t DEC_A();

    uint8_t LD_C_d8();
    uint8_t LD_E_d8();
    uint8_t LD_L_d8();
    uint8_t LD_A_d8();

    uint8_t RRCA();
    uint8_t RRA();
    uint8_t CPL();
    uint8_t CCF();

    uint8_t LD_B_B();
    uint8_t LD_B_C();
    uint8_t LD_B_D();
    uint8_t LD_B_E();
    uint8_t LD_B_H();
    uint8_t LD_B_L();
    uint8_t LD_B_mem_HL();
    uint8_t LD_B_A();
    uint8_t LD_C_B();
    uint8_t LD_C_C();
    uint8_t LD_C_D();
    uint8_t LD_C_E();
    uint8_t LD_C_H();
    uint8_t LD_C_L();
    uint8_t LD_C_mem_HL();
    uint8_t LD_C_A();
    uint8_t LD_D_B();
    uint8_t LD_D_C();
    uint8_t LD_D_D();
    uint8_t LD_D_E();
    uint8_t LD_D_H();
    uint8_t LD_D_L();
    uint8_t LD_D_mem_HL();
    uint8_t LD_D_A();
    uint8_t LD_E_B();
    uint8_t LD_E_C();
    uint8_t LD_E_D();
    uint8_t LD_E_E();
    uint8_t LD_E_H();
    uint8_t LD_E_L();
    uint8_t LD_E_mem_HL();
    uint8_t LD_E_A();
    uint8_t LD_H_B();
    uint8_t LD_H_C();
    uint8_t LD_H_D();
    uint8_t LD_H_E();
    uint8_t LD_H_H();
    uint8_t LD_H_L();
    uint8_t LD_H_mem_HL();
    uint8_t LD_H_A();
    uint8_t LD_L_B();
    uint8_t LD_L_C();
    uint8_t LD_L_D();
    uint8_t LD_L_E();
    uint8_t LD_L_H();
    uint8_t LD_L_L();
    uint8_t LD_L_mem_HL();
    uint8_t LD_L_A();
    uint8_t LD_mem_HL_B();
    uint8_t LD_mem_HL_C();
    uint8_t LD_mem_HL_D();
    uint8_t LD_mem_HL_E();
    uint8_t LD_mem_HL_H();
    uint8_t LD_mem_HL_L();
    uint8_t HALT();
    uint8_t LD_mem_HL_A();
    uint8_t LD_A_B();
    uint8_t LD_A_C();
    uint8_t LD_A_D();
    uint8_t LD_A_E();
    uint8_t LD_A_H();
    uint8_t LD_A_L();
    uint8_t LD_A_mem_HL();
    uint8_t LD_A_A();

    uint8_t ADD_A_B();
    uint8_t ADD_A_C();
    uint8_t ADD_A_D();
    uint8_t ADD_A_E();
    uint8_t ADD_A_H();
    uint8_t ADD_A_L();
    uint8_t ADD_A_mem_HL();
    uint8_t ADD_A_A();
    uint8_t ADC_A_B();
    uint8_t ADC_A_C();
    uint8_t ADC_A_D();
    uint8_t ADC_A_E();
    uint8_t ADC_A_H();
    uint8_t ADC_A_L();
    uint8_t ADC_A_mem_HL();
    uint8_t ADC_A_A();

    uint8_t SUB_A_B();
    uint8_t SUB_A_C();
    uint8_t SUB_A_D();
    uint8_t SUB_A_E();
    uint8_t SUB_A_H();
    uint8_t SUB_A_L();
    uint8_t SUB_A_mem_HL();
    uint8_t SUB_A_A();
    uint8_t SBC_A_B();
    uint8_t SBC_A_C();
    uint8_t SBC_A_D();
    uint8_t SBC_A_E();
    uint8_t SBC_A_H();
    uint8_t SBC_A_L();
    uint8_t SBC_A_mem_HL();
    uint8_t SBC_A_A();

    uint8_t AND_A_B();
    uint8_t AND_A_C();
    uint8_t AND_A_D();
    uint8_t AND_A_E();
    uint8_t AND_A_H();
    uint8_t AND_A_L();
    uint8_t AND_A_mem_HL();
    uint8_t AND_A_A();
    uint8_t XOR_A_B();
    uint8_t XOR_A_C();
    uint8_t XOR_A_D();
    uint8_t XOR_A_E();
    uint8_t XOR_A_H();
    uint8_t XOR_A_L();
    uint8_t XOR_A_mem_HL();
    uint8_t XOR_A_A();

    uint8_t OR_A_B();
    uint8_t OR_A_C();
    uint8_t OR_A_D();
    uint8_t OR_A_E();
    uint8_t OR_A_H();
    uint8_t OR_A_L();
    uint8_t OR_A_mem_HL();
    uint8_t OR_A_A();
    uint8_t CP_A_B();
    uint8_t CP_A_C();
    uint8_t CP_A_D();
    uint8_t CP_A_E();
    uint8_t CP_A_H();
    uint8_t CP_A_L();
    uint8_t CP_A_mem_HL();
    uint8_t CP_A_A();

    uint8_t RET_NZ();
    uint8_t RET_NC();
    uint8_t LDH_mem_n_A();
    uint8_t LDH_A_mem_n();

    uint8_t POP_BC();
    uint8_t POP_DE();
    uint8_t POP_HL();
    uint8_t POP_AF();

    uint8_t JP_NZ_nn();
    uint8_t JP_NC_nn();
    uint8_t LD_mem_C_A();
    uint8_t LDH_A_mem_C();

    uint8_t JP_nn();
    uint8_t DI();

    uint8_t CALL_NZ_nn();
    uint8_t CALL_NC_nn();

    uint8_t PUSH_BC();
    uint8_t PUSH_DE();
    uint8_t PUSH_HL();
    uint8_t PUSH_AF();

    uint8_t ADD_A_d8();
    uint8_t SUB_A_d8();
    uint8_t AND_A_d8();
    uint8_t OR_A_d8();

    uint8_t RST_00H();
    uint8_t RST_10H();
    uint8_t RST_20H();
    uint8_t RST_30H();

    uint8_t RET_Z();
    uint8_t RET_C();
    uint8_t ADD_SP_r8();
    uint8_t LD_HL_SP_r8();

    uint8_t RET();
    uint8_t RETI();
    uint8_t JP_HL();
    uint8_t LD_SP_HL();

    uint8_t JP_Z_nn();
    uint8_t JP_C_nn();
    uint8_t LD_mem_nn_A();
    uint8_t LD_A_mem_nn();

    uint8_t EI();

    uint8_t CALL_Z_nn();
    uint8_t CALL_C_nn();

    uint8_t CALL_nn();

    uint8_t ADC_A_d8();
    uint8_t SBC_A_d8();
    uint8_t XOR_A_d8();
    uint8_t CP_A_d8();

    uint8_t RST_08H();
    uint8_t RST_18H();
    uint8_t RST_28H();
    uint8_t RST_38H();

    // helper function for 0x20, 0x28, 0x30, 0x38
    uint8_t JR_i8(uint8_t condition);
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
    // helper function for 0x09, 0x19, 0x29
    uint8_t ADD_HL_RR(uint8_t reg1, uint8_t reg2);
    // helper function for 0x0A, 0x1A
    uint8_t LD_A_mem_RR(uint8_t reg1, uint8_t reg2);
    // helper function for 0x2A, 0x3A
    uint8_t LD_A_mem_HL_inc_dec(int8_t inc);
    // helper function for 0x0B, 0x1B, 0x2B
    uint8_t DEC_RR(uint8_t &reg1, uint8_t &reg2);
    // helper function for 0x0C, 0x1C, 0x2C, 0x3C
    uint8_t INC_R(uint8_t &reg);
    // helper function for 0x40 - 0x7F (minus 0x76) and LD MEM_HL_R
    uint8_t LD_R_R(uint8_t &reg1, uint8_t reg2);
    // helper function for 0x80 - 0x87 (minus 0x86)
    uint8_t ADD_A_R(uint8_t reg);
    // helper function for 0x88 - 0x8F (minus 0x8E)
    uint8_t ADC_A_R(uint8_t reg);
    // helper function for 0x90 - 0x97 (minus 0x96)
    uint8_t SUB_A_R(uint8_t reg);
    // helper function for 0x98 - 0x9F (minus 0x9E)
    uint8_t SBC_A_R(uint8_t reg);
    // helper function for 0xA0 - 0xA7 (minus 0xA6)
    uint8_t AND_A_R(uint8_t reg);
    // helper function for 0xA8 - 0xAF (minus 0xAE)
    uint8_t XOR_A_R(uint8_t reg);
    // helper function for 0xB0 - 0xB7 (minus 0xB6)
    uint8_t OR_A_R(uint8_t reg);
    // helper function for 0xB8 - 0xBF (minus 0xBE)
    uint8_t CP_A_R(uint8_t reg);
    // helper function for 0xC0, 0xD0
    uint8_t RET_i8(uint8_t condition);
    // helper function for 0xE0, 0xF0
    uint8_t LD_mem_FF00_plus_d8_A(uint8_t offset);
    // helper function for 0xC1, 0xD1, 0xE1, 0xF1
    uint8_t POP_RR(uint8_t &reg1, uint8_t &reg2);
    // helper function for 0xC2, 0xD2
    uint8_t JP_i16(uint8_t condition);
    // helper function for 0xE2, 0xF2
    uint8_t LD_mem_FF00_plus_C_A(uint8_t offset);
    // helper function for 0xC0, 0xD0
    uint8_t RET_cc(uint8_t flag);
    // helper function for 0xE0, 0xF0
    uint8_t LDH_mem_n_A(uint8_t offset);
    // helper function for 0xC2, 0xD2
    uint8_t JP_cc_i16(uint8_t condition);
    // helper function for 0xE2, 0xF2
    uint8_t LD_mem_C_A(uint8_t offset);
    // helper function for 0xC4, 0xD4
    uint8_t CALL_cc_i16(uint8_t condition);
    // helper function for 0xC5, 0xD5, 0xE5, 0xF5
    uint8_t PUSH_RR(uint8_t reg1, uint8_t reg2);
    // helper function for 0xC7, 0xD7, 0xE7, 0xF7
    uint8_t RST_n(uint8_t offset);
    // helper function for 0xCA, 0xDA
    uint8_t JP_cc_nn(uint8_t flag);
    // helper function for 0xCB
    uint8_t CB();
    // helper function for 0xCC, 0xDC
    uint8_t CALL_cc_nn(uint8_t flag);

    // CB
    uint8_t RLC_B();
    uint8_t RLC_C();
    uint8_t RLC_D();
    uint8_t RLC_E();
    uint8_t RLC_H();
    uint8_t RLC_L();
    uint8_t RLC_HL();
    uint8_t RLC_A();
    uint8_t RRC_B();
    uint8_t RRC_C();
    uint8_t RRC_D();
    uint8_t RRC_E();
    uint8_t RRC_H();
    uint8_t RRC_L();
    uint8_t RRC_HL();
    uint8_t RRC_A();
    uint8_t RL_B();
    uint8_t RL_C();
    uint8_t RL_D();
    uint8_t RL_E();
    uint8_t RL_H();
    uint8_t RL_L();
    uint8_t RL_HL();
    uint8_t RL_A();
    uint8_t RR_B();
    uint8_t RR_C();
    uint8_t RR_D();
    uint8_t RR_E();
    uint8_t RR_H();
    uint8_t RR_L();
    uint8_t RR_HL();
    uint8_t RR_A();
    uint8_t SLA_B();
    uint8_t SLA_C();
    uint8_t SLA_D();
    uint8_t SLA_E();
    uint8_t SLA_H();
    uint8_t SLA_L();
    uint8_t SLA_HL();
    uint8_t SLA_A();
    uint8_t SRA_B();
    uint8_t SRA_C();
    uint8_t SRA_D();
    uint8_t SRA_E();
    uint8_t SRA_H();
    uint8_t SRA_L();
    uint8_t SRA_HL();
    uint8_t SRA_A();
    uint8_t SWAP_B();
    uint8_t SWAP_C();
    uint8_t SWAP_D();
    uint8_t SWAP_E();
    uint8_t SWAP_H();
    uint8_t SWAP_L();
    uint8_t SWAP_HL();
    uint8_t SWAP_A();
    uint8_t SRL_B();
    uint8_t SRL_C();
    uint8_t SRL_D();
    uint8_t SRL_E();
    uint8_t SRL_H();
    uint8_t SRL_L();
    uint8_t SRL_HL();
    uint8_t SRL_A();
    uint8_t BIT_0_B();
    uint8_t BIT_0_C();
    uint8_t BIT_0_D();
    uint8_t BIT_0_E();
    uint8_t BIT_0_H();
    uint8_t BIT_0_L();
    uint8_t BIT_0_HL();
    uint8_t BIT_0_A();
    uint8_t BIT_1_B();
    uint8_t BIT_1_C();
    uint8_t BIT_1_D();
    uint8_t BIT_1_E();
    uint8_t BIT_1_H();
    uint8_t BIT_1_L();
    uint8_t BIT_1_HL();
    uint8_t BIT_1_A();
    uint8_t BIT_2_B();
    uint8_t BIT_2_C();
    uint8_t BIT_2_D();
    uint8_t BIT_2_E();
    uint8_t BIT_2_H();
    uint8_t BIT_2_L();
    uint8_t BIT_2_HL();
    uint8_t BIT_2_A();
    uint8_t BIT_3_B();
    uint8_t BIT_3_C();
    uint8_t BIT_3_D();
    uint8_t BIT_3_E();
    uint8_t BIT_3_H();
    uint8_t BIT_3_L();
    uint8_t BIT_3_HL();
    uint8_t BIT_3_A();
    uint8_t BIT_4_B();
    uint8_t BIT_4_C();
    uint8_t BIT_4_D();
    uint8_t BIT_4_E();
    uint8_t BIT_4_H();
    uint8_t BIT_4_L();
    uint8_t BIT_4_HL();
    uint8_t BIT_4_A();
    uint8_t BIT_5_B();
    uint8_t BIT_5_C();
    uint8_t BIT_5_D();
    uint8_t BIT_5_E();
    uint8_t BIT_5_H();
    uint8_t BIT_5_L();
    uint8_t BIT_5_HL();
    uint8_t BIT_5_A();
    uint8_t BIT_6_B();
    uint8_t BIT_6_C();
    uint8_t BIT_6_D();
    uint8_t BIT_6_E();
    uint8_t BIT_6_H();
    uint8_t BIT_6_L();
    uint8_t BIT_6_HL();
    uint8_t BIT_6_A();
    uint8_t BIT_7_B();
    uint8_t BIT_7_C();
    uint8_t BIT_7_D();
    uint8_t BIT_7_E();
    uint8_t BIT_7_H();
    uint8_t BIT_7_L();
    uint8_t BIT_7_HL();
    uint8_t BIT_7_A();
    uint8_t RES_0_B();
    uint8_t RES_0_C();
    uint8_t RES_0_D();
    uint8_t RES_0_E();
    uint8_t RES_0_H();
    uint8_t RES_0_L();
    uint8_t RES_0_HL();
    uint8_t RES_0_A();
    uint8_t RES_1_B();
    uint8_t RES_1_C();
    uint8_t RES_1_D();
    uint8_t RES_1_E();
    uint8_t RES_1_H();
    uint8_t RES_1_L();
    uint8_t RES_1_HL();
    uint8_t RES_1_A();
    uint8_t RES_2_B();
    uint8_t RES_2_C();
    uint8_t RES_2_D();
    uint8_t RES_2_E();
    uint8_t RES_2_H();
    uint8_t RES_2_L();
    uint8_t RES_2_HL();
    uint8_t RES_2_A();
    uint8_t RES_3_B();
    uint8_t RES_3_C();
    uint8_t RES_3_D();
    uint8_t RES_3_E();
    uint8_t RES_3_H();
    uint8_t RES_3_L();
    uint8_t RES_3_HL();
    uint8_t RES_3_A();
    uint8_t RES_4_B();
    uint8_t RES_4_C();
    uint8_t RES_4_D();
    uint8_t RES_4_E();
    uint8_t RES_4_H();
    uint8_t RES_4_L();
    uint8_t RES_4_HL();
    uint8_t RES_4_A();
    uint8_t RES_5_B();
    uint8_t RES_5_C();
    uint8_t RES_5_D();
    uint8_t RES_5_E();
    uint8_t RES_5_H();
    uint8_t RES_5_L();
    uint8_t RES_5_HL();
    uint8_t RES_5_A();
    uint8_t RES_6_B();
    uint8_t RES_6_C();
    uint8_t RES_6_D();
    uint8_t RES_6_E();
    uint8_t RES_6_H();
    uint8_t RES_6_L();
    uint8_t RES_6_HL();
    uint8_t RES_6_A();
    uint8_t RES_7_B();
    uint8_t RES_7_C();
    uint8_t RES_7_D();
    uint8_t RES_7_E();
    uint8_t RES_7_H();
    uint8_t RES_7_L();
    uint8_t RES_7_HL();
    uint8_t RES_7_A();
    uint8_t SET_0_B();
    uint8_t SET_0_C();
    uint8_t SET_0_D();
    uint8_t SET_0_E();
    uint8_t SET_0_H();
    uint8_t SET_0_L();
    uint8_t SET_0_HL();
    uint8_t SET_0_A();
    uint8_t SET_1_B();
    uint8_t SET_1_C();
    uint8_t SET_1_D();
    uint8_t SET_1_E();
    uint8_t SET_1_H();
    uint8_t SET_1_L();
    uint8_t SET_1_HL();
    uint8_t SET_1_A();
    uint8_t SET_2_B();
    uint8_t SET_2_C();
    uint8_t SET_2_D();
    uint8_t SET_2_E();
    uint8_t SET_2_H();
    uint8_t SET_2_L();
    uint8_t SET_2_HL();
    uint8_t SET_2_A();
    uint8_t SET_3_B();
    uint8_t SET_3_C();
    uint8_t SET_3_D();
    uint8_t SET_3_E();
    uint8_t SET_3_H();
    uint8_t SET_3_L();
    uint8_t SET_3_HL();
    uint8_t SET_3_A();
    uint8_t SET_4_B();
    uint8_t SET_4_C();
    uint8_t SET_4_D();
    uint8_t SET_4_E();
    uint8_t SET_4_H();
    uint8_t SET_4_L();
    uint8_t SET_4_HL();
    uint8_t SET_4_A();
    uint8_t SET_5_B();
    uint8_t SET_5_C();
    uint8_t SET_5_D();
    uint8_t SET_5_E();
    uint8_t SET_5_H();
    uint8_t SET_5_L();
    uint8_t SET_5_HL();
    uint8_t SET_5_A();
    uint8_t SET_6_B();
    uint8_t SET_6_C();
    uint8_t SET_6_D();
    uint8_t SET_6_E();
    uint8_t SET_6_H();
    uint8_t SET_6_L();
    uint8_t SET_6_HL();
    uint8_t SET_6_A();
    uint8_t SET_7_B();
    uint8_t SET_7_C();
    uint8_t SET_7_D();
    uint8_t SET_7_E();
    uint8_t SET_7_H();
    uint8_t SET_7_L();
    uint8_t SET_7_HL();
    uint8_t SET_7_A();

    // CB helper functions
    uint8_t RLC_R(uint8_t &reg);
    uint8_t RRC_R(uint8_t &reg);
    uint8_t RL_R(uint8_t &reg);
    uint8_t RR_R(uint8_t &reg);
    uint8_t SLA_R(uint8_t &reg);
    uint8_t SRA_R(uint8_t &reg);
    uint8_t SWAP_R(uint8_t &reg);
    uint8_t SRL_R(uint8_t &reg);
    uint8_t BIT_b_R(uint8_t bit, uint8_t reg);
    uint8_t RES_b_R(uint8_t bit, uint8_t &reg);
    uint8_t SET_b_R(uint8_t bit, uint8_t &reg);

private:
    std::unique_ptr<FileReader> fileReader;

    // CPU registers
    uint8_t A, F;
    uint8_t B, C, D, E, H, L;
    uint16_t SP;
    uint16_t PC;

    // Memory management component
    Memory &memory;

    // Private methods for CPU functionality
    uint8_t executeInstruction(uint8_t opcode);
    void add(uint8_t value);
    void sub(uint8_t value);
    void logStatus();
};

#endif // CPU_H
