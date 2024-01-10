#include "../cartridge/cartridge.hpp"
#include "MBC1.hpp"

MBC1::MBC1(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize)
{
    this->rom = rom;
    this->ram = ram;
    romBank = 1;        // Initial ROM bank
    ramBank = 0;        // Initial RAM bank
    ramEnabled = false; // RAM initially disabled
    romBankingMode = 1; // ROM banking mode initially enabled
    this->romSize = romSize;
    this->ramSize = ramSize;
}

MBC1::~MBC1()
{
}

uint8_t MBC1::read(uint16_t address)
{
}

void MBC1::write(uint16_t address, uint8_t value)
{
}
