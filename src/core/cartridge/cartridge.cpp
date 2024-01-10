#include "cartridge.hpp"

Cartridge::Cartridge()
{
    rom = new uint8_t[0x8000];
    ram = new uint8_t[0x2000];
}

Cartridge::~Cartridge()
{
    if (rom)
    {
        delete[] rom;
    }
    if (ram)
    {
        delete[] ram;
    }
}

cartridgeHeader Cartridge::getHeader()
{
    return header;
}

uint8_t *Cartridge::getRom()
{
    return rom;
}

void Cartridge::setHeader(cartridgeHeader *header, uint8_t *rom)
{
    memcpy(header->entry, rom, 4);
    memcpy(header->nintendoLogo, rom + 0x104, 48);
    memcpy(header->title, rom + 0x134, 16);
    memcpy(header->manufacturerCode, rom + 0x13F, 4);
    header->cgbFlag = rom[0x143];
    memcpy(header->newLicenseeCode, rom + 0x144, 2);
    header->sgbFlag = rom[0x146];
    header->cartridgeType = rom[0x147];
    header->romSize = rom[0x148];
    header->ramSize = rom[0x149];
    header->destinationCode = rom[0x14A];
    header->oldLicenseeCode = rom[0x14B];
    header->maskRomVersionNumber = rom[0x14C];
    header->headerChecksum = rom[0x14D];
    header->globalChecksum = rom[0x14E] << 8 | rom[0x14F];
}

// load rom from file
void Cartridge::loadRom(char *buffer)
{
    // Load ROM implementation
    for (int i = 0; i < 0x4000; i++)
    {
        rom[i] = buffer[i];
    }

    // Load switchable ROM bank
    for (int i = 0, j = 0x4000; i < 0x4000; ++i, ++j)
    {
        rom[j] = buffer[j];
    }

    setHeader(&header, rom);
}

uint8_t Cartridge::read(uint16_t address)
{
    if (address < 0x4000)
    {
        return rom[address];
    }
    else if (address >= 0x4000 && address < 0x8000)
    {
        return rom[address];
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        return ram[address - 0xA000];
    }
    else
    {
        std::cerr << "Invalid cartridge read address: " << std::hex << address << std::endl;
        return 0;
    }
}

void Cartridge::write(uint16_t address, uint8_t value)
{
    if (address >= 0xA000 && address < 0xC000)
    {
        ram[address - 0xA000] = value;
    }
    else
    {
        std::cerr << "Invalid cartridge write address: " << std::hex << address << std::endl;
    }
}
