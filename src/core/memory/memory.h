#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include "../../logger/logger.h"

class Memory
{
public:
    Memory();
    ~Memory();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    void reset();
    void loadRom(char *data);

private:
    uint8_t *romBank;           // Fixed ROM bank (16KB)
    uint8_t *switchableRomBank; // Switchable ROM bank (16KB)
    uint8_t *cartridgeRam;      // Cartridge RAM (8KB)
    uint8_t *ioRegisters;       // I/O registers (128B)
    uint8_t *highRam;           // High RAM (127B)
    uint8_t *oam;               // OAM (160B)
    uint8_t *wram;              // Working RAM (8KB)
    uint8_t *vram;              // Video RAM (8KB)
};

#endif // MEMORY_H
