#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <FS.h>
#include <SD.h>

class Memory {
public:
    Memory();
    ~Memory();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    void reset();
    void loadRom(File romFile);
    
private:
    uint8_t romBank[0x4000]; // Fixed ROM bank (16KB)
    uint8_t ioRegisters[128]; // I/O registers (128B)
    uint8_t highRam[127];    // High RAM (127B)
    uint8_t oam[160];        // OAM (160B)
};

#endif // MEMORY_H
