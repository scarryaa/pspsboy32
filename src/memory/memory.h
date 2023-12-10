#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

class Memory {
public:
    Memory();
    ~Memory();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t value);
    void reset();

private:
};

#endif // MEMORY_H
