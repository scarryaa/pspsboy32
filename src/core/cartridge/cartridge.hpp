#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>
#include <wchar.h>
#include <vector>
#include <fstream>
#include "core/mbc/ROM_ONLY.hpp"
#include "core/mbc/IMBC.hpp"
#include "core/mbc/MBC1.hpp"
#include "core/mbc/MBC2.hpp"
#include "core/mbc/size.hpp"

enum class CartridgeType
{
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC4 = 0x15,
    MBC4_RAM = 0x16,
    MBC4_RAM_BATTERY = 0x17,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    MBC6 = 0x20,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HuC3 = 0xFE,
    HuC1_RAM_BATTERY = 0xFF,
    UNKNOWN = 0x100
};

typedef struct
{
    uint8_t entry[4];
    uint8_t nintendoLogo[48];
    char title[16];
    uint8_t manufacturerCode[4];
    uint8_t cgbFlag;
    uint8_t newLicenseeCode[2];
    uint8_t sgbFlag;
    uint8_t cartridgeType;
    RomSize romSize;
    RamSize ramSize;
    uint8_t destinationCode;
    uint8_t oldLicenseeCode;
    uint8_t maskRomVersionNumber;
    uint8_t headerChecksum;
    uint16_t globalChecksum;
} cartridgeHeader;

class Cartridge
{
private:
    cartridgeHeader header;
    std::vector<uint8_t> rom;
    uint8_t *ram;

public:
    Cartridge();
    ~Cartridge();

    std::unique_ptr<IMBC> mbcHandler;
    void loadRom(const std::string &path);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    cartridgeHeader getHeader();
    std::vector<uint8_t> getRom();
    void setHeader(cartridgeHeader *header, uint8_t *rom);
};

#endif