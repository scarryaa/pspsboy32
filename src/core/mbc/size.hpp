#ifndef _SIZE_H_
#define _SIZE_H_

enum class RomSize
{
    _32KB = 0x00,  // 0 banks
    _64KB = 0x01,  // 4 banks
    _128KB = 0x02, // 8 banks
    _256KB = 0x03, // 16 banks
    _512KB = 0x04, // 32 banks
    _1MB = 0x05,   // 64 banks
    _2MB = 0x06,   // 128 banks
    _4MB = 0x07,   // 256 banks
    _8MB = 0x08,   // 512 banks
    _1_1MB = 0x52, // 72 banks
    _1_2MB = 0x53, // 80 banks
    _1_5MB = 0x54  // 96 banks
};

enum class RamSize
{
    NONE = 0x00,
    _2KB = 0x01,
    _8KB = 0x02,
    _32KB = 0x03,
    _128KB = 0x04,
    _64KB = 0x05
};

#endif