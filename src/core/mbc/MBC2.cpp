#include "core/mbc/MBC2.hpp"

MBC2::MBC2(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize)
{
    this->rom = rom;
    this->ram = ram;
    romBank = 1;
    ramEnabled = false;
    this->romSize = romSize;
    this->ramSize = ramSize;
}

MBC2::~MBC2()
{
}

uint8_t MBC2::read(uint16_t address)
{
}

void MBC2::write(uint16_t address, uint8_t value)
{
}