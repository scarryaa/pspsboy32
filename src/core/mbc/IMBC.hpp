#ifndef _IMBC_H_
#define _IMBC_H_

#include <cstdint>

class IMBC
{
public:
    virtual uint8_t read(uint16_t address) = 0;
    virtual void write(uint16_t address, uint8_t value) = 0;
    virtual void reset() = 0;
};

#endif