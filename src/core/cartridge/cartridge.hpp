#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>
#include <wchar.h>

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
    uint8_t romSize;
    uint8_t ramSize;
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
    uint8_t *rom;
    uint8_t *ram;

public:
    Cartridge();
    ~Cartridge();

    void loadRom(char *buffer);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    cartridgeHeader getHeader();
    uint8_t *getRom();
    void setHeader(cartridgeHeader *header, uint8_t *rom);
};

#endif