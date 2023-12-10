#include "cpu.h"

CPU::CPU() {
    // Constructor implementation
}

CPU::~CPU() {
    // Destructor implementation
}

void CPU::reset() {
    // Reset implementation

    // Set the program counter to the start of the ROM
    // Set the stack pointer to the start of the stack
    // Set the registers to their initial values
    // Set the flags to their initial values
}

void CPU::executeCycle() {
    // Execute cycle implementation
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
    // Execute instruction implementation
}

void CPU::add(uint8_t value) {
    // Add implementation
}

void CPU::sub(uint8_t value) {
    // Subtract implementation
}

void CPU::opcode00() {
    // Opcode 00 implementation
}

void CPU::opcode01() {
    // Opcode 01 implementation
}
