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
    header->romSize = RomSize(rom[0x148]);
    header->ramSize = RamSize(rom[0x149]);
    header->destinationCode = rom[0x14A];
    header->oldLicenseeCode = rom[0x14B];
    header->maskRomVersionNumber = rom[0x14C];
    header->headerChecksum = rom[0x14D];
    header->globalChecksum = rom[0x14E] << 8 | rom[0x14F];

    uint8_t cartridgeType = rom[0x147];

    switch (cartridgeType)
    {
    case 0x00:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::ROM_ONLY);
        break;
    case 0x01:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::MBC1);
        break;
    case 0x02:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::MBC1);
        break;
    default:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::UNKNOWN);
        break;
    }
}

// load rom from file
void Cartridge::loadRom(char *buffer)
{
    memcpy(rom, buffer, 0x8000);
    setHeader(&header, rom);

    switch (header.cartridgeType)
    {
    case static_cast<uint8_t>(CartridgeType::ROM_ONLY):
        mbcHandler = std::unique_ptr<IMBC>(new ROM_ONLY(rom, ram, header.romSize, header.ramSize));
        break;
    case static_cast<uint8_t>(CartridgeType::MBC1):
        mbcHandler = std::unique_ptr<IMBC>(new MBC1(rom, ram, header.romSize, header.ramSize));
        break;
    default:
        break;
    }
}

uint8_t Cartridge::read(uint16_t address)
{
    if (mbcHandler)
    {
        return mbcHandler->read(address);
    }
    else
    {
        // Fallback or error handling
        return 0;
    }
}

void Cartridge::write(uint16_t address, uint8_t value)
{
    if (mbcHandler)
    {
        mbcHandler->write(address, value);
    }
    else
    {
        // Fallback or error handling
    }
}