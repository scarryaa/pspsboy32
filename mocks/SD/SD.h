#include <iostream>
#include "../SPI/SPI.h"
#include "../File/File.h"

#ifndef _FS_H_
#define _FS_H_

#define FILE_APPEND 0

class SDClass
{
public:
    bool begin(uint8_t csPin);

    bool begin(int, SPIClass &, int, const char[1], int);

    bool exists(const char *filepath);

    File open(const char *filepath);

    File open(const char *filepath, uint8_t mode);

    void end();
};

extern SDClass SD;

#endif // _FS_H_