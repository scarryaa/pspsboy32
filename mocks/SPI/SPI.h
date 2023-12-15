#include <stdint.h>

#ifndef _SPI_H_
#define _SPI_H_

// mock SPI.h
#define FSPI 1 // SPI bus attached to the flash (can use the same data lines but different SS)
#define HSPI 2 // SPI bus normally mapped to pins 12 - 15, but can be matrixed to any pins

#define SPI_HAS_TRANSACTION
#define SPI_MODE0 0x00 ///< CPOL: 0  CPHA: 0
#define SPI_MODE1 0x01 ///< CPOL: 0  CPHA: 1
#define SPI_MODE2 0x02 ///< CPOL: 1  CPHA: 0
#define SPI_MODE3 0x03 ///< CPOL: 1  CPHA: 1
#define SPI_MSBFIRST 0
#define SPI_LSBFIRST 1

class SPISettings
{
public:
    SPISettings() : _clock(1000000), _bitOrder(SPI_MSBFIRST), _dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}
    uint32_t _clock;
    uint8_t _bitOrder;
    uint8_t _dataMode;
};

class SPIClass
{
public:
    SPIClass(uint8_t spi_bus = HSPI);
    ~SPIClass();
    void begin(int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, int8_t ss = -1);
    void end();

    void setHwCs(bool use);
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setFrequency(uint32_t freq);
    void setClockDivider(uint32_t clockDiv);

    uint32_t getClockDivider();

    void beginTransaction(SPISettings settings);
    void endTransaction(void);
    void transfer(void *data, uint32_t size);
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    uint32_t transfer32(uint32_t data);

    void transferBytes(const uint8_t *data, uint8_t *out, uint32_t size);
    void transferBits(uint32_t data, uint32_t *out, uint8_t bits);

    void write(uint8_t data);
    void write16(uint16_t data);
    void write32(uint32_t data);
};

#endif // __SPI_H__