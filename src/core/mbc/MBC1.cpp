#include "../cartridge/cartridge.hpp"
#include "MBC1.hpp"

MBC1::MBC1(std::vector<uint8_t> rom, uint8_t *ram, RomSize romSize, RamSize ramSize)
{
    // up to 125 banks of 16KB each (2MB)
    this->rom = rom;
    this->ram = new uint8_t[0x2000 * static_cast<uint8_t>(ramSize)];
    this->romSize = romSize;
    this->ramSize = ramSize;
    this->mbc1Reg = 0x01;   // MBC bank register 1, 5 bits
    this->mbc2Reg = 0;      // MBC bank register 2, 2 bits
    this->mbcMode = 0;      // ROM Banking Mode,    1 bit
    this->mbcRamEnable = 0; // Disable RAM by default
}

MBC1::~MBC1()
{
    delete[] ram;
}

uint8_t MBC1::read(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3FFF)
    {
        if (mbcMode == 0)
        {
            // ROM bank 0
            return rom[address];
        }
        else
        {
            // ROM bank ??
            return rom[(mbc2Reg & 0x03) << 19 | address];
        }
    }
    else if (address >= 0x4000 && address <= 0x7FFF)
    {
        size_t romBankIndex = (mbc2Reg & 0x03) << 19 | (mbc1Reg & 0x1F) << 14 | address - 0x4000;
        return rom[romBankIndex];
    }

    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        if (mbcRamEnable == 0)
        {
            // RAM disabled
            return 0xFF;
        }
        else
        {
            // RAM bank ??
            return ram[(mbc2Reg & 0x03) << 13 | address - 0xA000];
        }
    }
    else
    {
        // Fallback or error handling
        return 0;
    }
}

void MBC1::write(uint16_t address, uint8_t value)
{
    if (address >= 0x0000 && address <= 0x1FFF)
    {
        // RAM Enable
        // 0x0A = RAM enabled
        // 0x00 = RAM disabled
        mbcRamEnable = (value & 0x0F) == 0x0A ? 1 : 0;
    }
    else if (address >= 0x2000 && address <= 0x3FFF)
    {
        // MBC bank register 1
        mbc1Reg = value & 0x1F;
        mbc1Reg = (mbc1Reg == 0) ? 1 : mbc1Reg;
    }
    else if (address >= 0x4000 && address <= 0x5FFF)
    {
        // MBC bank register 2
        mbc2Reg = value & 0x03;
    }
    else if (address >= 0x6000 && address <= 0x7FFF)
    {
        // ROM/RAM Mode Select
        // 0x00 = ROM Banking Mode (up to 8KB RAM, 2MB ROM)
        // 0x01 = RAM Banking Mode (up to 32KB RAM, 512KB ROM)
        mbcMode = (value & 0x0F) == 0x01 ? 1 : 0;
    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        if ((mbcRamEnable & 0x0F) == 0x0A)
        {
            // RAM bank ??
            ram[mbc2Reg << 13 | address - 0xA000] = value;
        }
    }

    else
    {
        // Fallback or error handling
    }
}
