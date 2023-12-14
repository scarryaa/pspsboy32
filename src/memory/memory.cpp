#include "memory.h"

Memory::Memory() {

}

void Memory::loadRom() {
    if (!SD.exists("/roms/04-op r,imm.gb")) {
        Serial.println("ROM file does not exist");
        return;
    }

    File romFile = SD.open("/roms/03-op sp,hl.gb");
    if (!romFile) {
        Serial.println("Error opening ROM file");
        return;
    }

    size_t bytesRead = romFile.readBytes((char*)romBank, sizeof(romBank));

    Serial.print(bytesRead);
    Serial.println(" bytes read into ROM bank");

    Serial.print("ROM bank contents: ");
    for (int i = 0; i < sizeof(romBank); i++) {
        Serial.print(readByte(i), HEX);
        Serial.print(" ");
    }
    Serial.println();

    Serial.println("ROM file read OK");
    romFile.close();
}



Memory::~Memory() {
}

uint8_t Memory::readByte(uint16_t address) {
    if (address < 0x4000) {
        // Read from fixed ROM bank
        // Serial.print("Reading address ");
        // Serial.print(address, HEX);
        // Serial.print(" from fixed ROM bank: ");
        // Serial.println(romBank[address], HEX);
        return romBank[address];
    } else if (address >= 0x8000 && address < 0xA000) {
        // Read from VRAM
    } 

    // Default
    return 0;
}

void Memory::writeByte(uint16_t address, uint8_t value) {
    if (address == 0xFF01) {
        // Write to SB (Serial transfer data)
        ioRegisters[0x01] = value;
    } else if (address == 0xFF02) {
        // Write to SC (Serial transfer control)
        ioRegisters[0x02] = value;
        if (value == 0x81) {
            // When 0x81 is written to SC, send the character in SB to Serial
            Serial.write(ioRegisters[0x01]);
        }
    }

    if (address >= 0x8000 && address < 0xA000) {
        // Write to VRAM
    } else if (address >= 0xC000 && address < 0xE000) {
        // Write to WRAM
    } else if (address >= 0xFE00 && address < 0xFEA0) {
        // Write to OAM
        oam[address - 0xFE00] = value;
    } else if (address >= 0xFF00 && address < 0xFF80) {
        // Write to I/O registers
        ioRegisters[address - 0xFF00] = value;
    } else if (address >= 0xFF80) {
        // Write to high RAM
        highRam[address - 0xFF80] = value;
    }
}

void Memory::reset() {
    // Reset ROM bank
    for (int i = 0; i < sizeof(romBank); i++) {
        romBank[i] = 0;
    }

    // Reset I/O registers
    for (int i = 0; i < sizeof(ioRegisters); i++) {
        ioRegisters[i] = 0;
    }
}
