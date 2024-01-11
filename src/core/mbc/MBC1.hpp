#ifndef _MBC1_H_
#define _MBC1_H_

#include <cstdint>
#include "core/mbc/IMBC.hpp"
#include "core/mbc/size.hpp"
#include <stdbool.h>

class MBC1 : public IMBC
{
private:
    std::vector<uint8_t> rom;
    uint8_t *ram;
    uint8_t mbc1Reg;
    uint8_t mbc2Reg;
    uint8_t mbcMode;
    bool mbcRamEnable;
    RomSize romSize;
    RamSize ramSize;

public:
    MBC1(std::vector<uint8_t> rom, uint8_t *ram, RomSize romSize, RamSize ramSize);
    ~MBC1();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};

#endif