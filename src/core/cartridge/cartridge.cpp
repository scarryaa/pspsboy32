#include "cartridge.hpp"

Cartridge::Cartridge()
{
}

Cartridge::~Cartridge()
{
}

cartridgeHeader Cartridge::getHeader()
{
    return header;
}

std::vector<uint8_t> Cartridge::getRom()
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
    case 0x03:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::MBC1);
        break;
    default:
        header->cartridgeType = static_cast<uint8_t>(CartridgeType::UNKNOWN);
        break;
    }
}

// load rom from file
void Cartridge::loadRom(const std::string &path)
{
    FILE *file = std::fopen(path.c_str(), "rb");
    if (!file)
    {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::fseek(file, 0, SEEK_END);
    size_t size = std::ftell(file);
    std::rewind(file);

    // Resize the vector to fit the ROM
    rom.resize(size);

    // Read the file into the vector
    std::fread(rom.data(), sizeof(uint8_t), size, file);

    // Close the file
    std::fclose(file);

    setHeader(&header, rom.data());

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