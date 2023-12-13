#include "cpu.h"

typedef uint8_t (CPU::*InstructionFunc)();
InstructionFunc instructionTable[256];
bool CPU::stopped = false;

CPU::CPU() {
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
}

CPU::~CPU() {
    // Destructor implementation
}

void CPU::reset() {
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

    // Set the flags to their initial values
    f_Z = false;
    f_N = false;
    f_H = false;
    f_C = false;
}

uint8_t CPU::fetchInstruction() {
    // Fetch the next instruction from memory
    uint8_t opcode = memory.readByte(PC);

    return opcode;
}

void CPU::executeExtendedInstruction(uint8_t opcode) {
    // Extended instruction implementation
}

void CPU::executeCycle() {
    // Fetch the next instruction
    uint8_t opcode = fetchInstruction();

    // Handle prefixed opcodes
    if (opcode == 0xCB) {
        opcode = fetchInstruction();
        executeExtendedInstruction(opcode);
    } else {
        // Decode and execute the instruction
        executeInstruction(opcode);
    }

    // Update the timers
    updateTimers();

    // Handle interrupts
    handleInterrupts();
}

void CPU::handleInterrupts() {
    // Handle interrupts implementation
}

void CPU::setFlag() {
    // Set flag implementation
}

bool CPU::checkFlag() {
    // Check flag implementation

    return false;
}

void CPU::executeInstruction(uint8_t opcode) {
    InstructionFunc func = instructionTable[opcode];
    uint8_t increment = (this->*func)();
    PC += increment;
}

void CPU::add(uint8_t value) {
    // Add implementation
}

void CPU::sub(uint8_t value) {
    // Subtract implementation
}

void CPU::updateTimers() {
    // Update any relevant timers or counters
}