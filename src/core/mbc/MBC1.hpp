#ifndef _MBC1_H_
#define _MBC1_H_

#include <cstdint>
#include "core/mbc/IMBC.hpp"
#include "core/mbc/size.hpp"

class MBC1 : public IMBC
{
private:
    uint8_t *rom;
    uint8_t *ram;
    uint8_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
    int romBankingMode;
    RomSize romSize;
    RamSize ramSize;

public:
    MBC1(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize);
    ~MBC1();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
};

#endif