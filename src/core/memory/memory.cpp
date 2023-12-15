#include "memory.h"

Memory::Memory()
{
    ioRegisters = new uint8_t[128];
    highRam = new uint8_t[127];
    oam = new uint8_t[160];
    wram = new uint8_t[0x2000];
    vram = new uint8_t[0x2000];
    cartridgeRam = new uint8_t[0x2000];
}

// load rom from data
void Memory::loadRom(char *data)
{
    // Load ROM implementation
    for (int i = 0; i < sizeof(romBank); i++)
    {
        romBank[i] = data[i];
    }

    // Load switchable ROM bank
    for (int i = 0; i < sizeof(switchableRomBank); i++)
    {
        switchableRomBank[i] = data[i];
    }
}

Memory::~Memory()
{
    delete[] ioRegisters;
    delete[] highRam;
    delete[] oam;
    delete[] vram;
    delete[] wram;
    delete[] cartridgeRam;
}

uint8_t Memory::readByte(uint16_t address)
{
    if (address < 0x4000)
    {
        // Read from fixed ROM bank
        return romBank[address];
    }
    else if (address >= 0x4000 && address < 0x8000)
    {
        // Read from switchable ROM bank
        return switchableRomBank[address - 0x4000];
    }
    else if (address >= 0xC000 && address < 0xE000)
    {
        // Read from WRAM
        return wram[address - 0xC000];
    }
    else if (address >= 0xE000 && address < 0xFE00)
    {
        // Read from ECHO RAM
        return wram[address - 0xE000];
    }
    else if (address >= 0xFE00 && address < 0xFEA0)
    {
        // Read from OAM
        return oam[address - 0xFE00];
    }
    else if (address >= 0xFF00 && address < 0xFF80)
    {
        // Read from I/O registers
        return ioRegisters[address - 0xFF00];
    }
    else if (address >= 0xFF80)
    {
        // Read from high RAM
        return highRam[address - 0xFF80];
    }
    else if (address >= 0x8000 && address < 0xA000)
    {
        // Read from VRAM
        return vram[address - 0x8000];
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        // Read from cartridge RAM
        return cartridgeRam[address - 0xA000];
    }
    else if (address >= 0xFEA0 && address < 0xFF00)
    {
        // Unusable memory
        return 0xFF;
    }
    else if (address == 0xFFFF)
    {
        // Interrupt enable register
    }

    // Default
    return 0;
}

void Memory::writeByte(uint16_t address, uint8_t value)
{
    if (address == 0xFF01)
    {
        // Write to SB (Serial transfer data)
        ioRegisters[0x01] = value;
    }
    else if (address == 0xFF02)
    {
        // Write to SC (Serial transfer control)
        ioRegisters[0x02] = value;
        if (value == 0x81)
        {
            // When 0x81 is written to SC, send the character in SB to Serial
            // Serial.write(ioRegisters[0x01]);
        }
    }

    if (address >= 0x8000 && address < 0xA000)
    {
        // Write to VRAM
        vram[address - 0x8000] = value;
    }
    else if (address >= 0xC000 && address < 0xE000)
    {
        // Write to WRAM
        wram[address - 0xC000] = value;
    }
    else if (address >= 0xE000 && address < 0xFE00)
    {
        // Write to ECHO RAM
        wram[address - 0xE000] = value;
    }
    else if (address >= 0xFE00 && address < 0xFEA0)
    {
        // Write to OAM
        oam[address - 0xFE00] = value;
    }
    else if (address >= 0xFF00 && address < 0xFF80)
    {
        // Write to I/O registers
        ioRegisters[address - 0xFF00] = value;
    }
    else if (address >= 0xFF80)
    {
        // Write to high RAM
        highRam[address - 0xFF80] = value;
    }
    else if (address >= 0xFEA0 && address < 0xFF00)
    {
        // Unusable memory
    }
    else if (address == 0xFFFF)
    {
        // Interrupt enable register
    }
}

void Memory::reset()
{
    // Reset ROM bank
    for (int i = 0; i < sizeof(romBank); i++)
    {
        romBank[i] = 0;
    }

    // Reset I/O registers
    for (int i = 0; i < sizeof(ioRegisters); i++)
    {
        ioRegisters[i] = 0;
    }
}
