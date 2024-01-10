#include <cstdint>
#include "core/mbc/IMBC.hpp"
#include "core/mbc/size.hpp"

class ROM_ONLY : public IMBC
{
private:
    uint8_t *rom;
    uint8_t *ram;
    RomSize romSize;
    RamSize ramSize;

public:
    ROM_ONLY(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize);
    ~ROM_ONLY();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};
