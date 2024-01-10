#include "ROM_ONLY.hpp"

ROM_ONLY::ROM_ONLY(uint8_t *rom, uint8_t *ram, RomSize romSize, RamSize ramSize)
{
    this->rom = rom;
    this->ram = ram;
    this->romSize = romSize;
    this->ramSize = ramSize;
}

ROM_ONLY::~ROM_ONLY()
{
}

uint8_t ROM_ONLY::read(uint16_t address)
{
    if (address < 0x8000)
    {
        return rom[address];
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        return ram[address - 0xA000];
    }
    else
    {
        return 0xFF;
    }
}

void ROM_ONLY::write(uint16_t address, uint8_t value)
{
    if (address >= 0xA000 && address < 0xC000)
    {
        ram[address - 0xA000] = value;
    }
}