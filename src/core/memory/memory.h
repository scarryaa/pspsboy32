#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include "../../logger/logger.h"
#include "../cartridge/cartridge.hpp"

#define OAM_START_ADDRESS 0xFE00

class Memory
{
public:
    Memory(Cartridge &cartridge);
    ~Memory();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    void reset();
    void loadRom(char *data, size_t size);

private:
    uint8_t *ioRegisters; // I/O registers (128B)
    uint8_t *highRam;     // High RAM (127B)
    uint8_t *oam;         // OAM (160B)
    uint8_t *wram;        // Working RAM (8KB)
    uint8_t *vram;        // Video RAM (8KB)

    uint8_t IE; // Interrupt enable register
    uint8_t IF; // Interrupt flag register

    Cartridge *cartridge;
};

#endif // MEMORY_H
