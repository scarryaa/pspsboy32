#include "cpu.h"

typedef uint8_t (CPU::*InstructionFunc)();
InstructionFunc instructionTable[256];

// CB prefixed instructions
typedef uint8_t (CPU::*CBInstructionFunc)();
CBInstructionFunc CBInstructionTable[256];

bool CPU::stopped = false;
bool CPU::halted = false;
bool CPU::IME = false;

CPU::CPU(Memory &memory) : memory(memory)
{
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
    instructionTable[0xCB] = &CPU::INVALID;
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
}

uint8_t CPU::fetchInstruction()
{
    // Fetch the next instruction from memory
    uint8_t opcode = memory.readByte(PC);
    // Serial.print("Fetched instruction: ");
    // Serial.println(opcode, HEX);

    return opcode;
}

void CPU::executeExtendedInstruction(uint8_t opcode)
{
    // Extended instruction implementation
}

void CPU::executeCycle()
{
    // Fetch the next instruction
    uint8_t opcode = fetchInstruction();

    // Handle prefixed opcodes
    if (opcode == 0xCB)
    {
        opcode = fetchInstruction();
        executeExtendedInstruction(opcode);
    }
    else
    {
        // Decode and execute the instruction
        executeInstruction(opcode);
    }

    // Update the timers
    updateTimers();

    // Handle interrupts
    handleInterrupts();
}

void CPU::handleInterrupts()
{
    // Handle interrupts implementation
}

void CPU::setFlag()
{
    // Set flag implementation
}

bool CPU::checkFlag()
{
    // Check flag implementation

    return false;
}

// void CPU::printStatus()
// {
//     char buffer[200];

//     uint8_t mem0 = memory.readByte(PC);
//     uint8_t mem1 = memory.readByte(PC + 1);
//     uint8_t mem2 = memory.readByte(PC + 2);
//     uint8_t mem3 = memory.readByte(PC + 3);

//     // Format the string with the register values and memory contents in hexadecimal format
//     snprintf(buffer, sizeof(buffer),
//              "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X",
//              A, F, B, C, D, E, H, L, SP, PC, mem0, mem1, mem2, mem3);

//     File file = SD.open("/status.txt", FILE_APPEND);
//     if (file)
//     {
//         file.println(buffer);
//         file.close();
//     }
//     else
//     {
//         Serial.println("Error opening status.txt");
//     }
// }

void CPU::executeInstruction(uint8_t opcode)
{
    // if (opcode != 0) {
    //     Serial.print("Executing instruction: ");
    //     Serial.println(opcode, HEX);
    // }

    // printStatus();

    InstructionFunc func = instructionTable[opcode];
    uint8_t cycles = (this->*func)();
}

void CPU::add(uint8_t value)
{
    // Add implementation
}

void CPU::sub(uint8_t value)
{
    // Subtract implementation
}

void CPU::updateTimers()
{
    // Update any relevant timers or counters
}