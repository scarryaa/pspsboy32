#include <cstdint>
#include "core/mbc/IMBC.hpp"
#include "core/mbc/size.hpp"

class MBC2 : public IMBC
{
private:
    uint8_t *rom;
    uint8_t *ram;
    uint8_t romBank;
    bool ramEnabled;
    RomSize romSize;
    RamSize ramSize;

public:
    MBC2(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize);
    ~MBC2();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};