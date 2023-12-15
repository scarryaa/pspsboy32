#include "SPI.h"
#include <iostream>

// Constructor for SPIClass
SPIClass::SPIClass(uint8_t spi_bus)
{
    std::cout << "SPI Class constructed with bus: " << (int)spi_bus << std::endl;
}

// Destructor for SPIClass
SPIClass::~SPIClass()
{
    std::cout << "SPI Class destructed" << std::endl;
}

void SPIClass::begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
{
    std::cout << "SPI begin called with SCK: " << sck << ", MISO: " << miso << ", MOSI: " << mosi << ", SS: " << ss << std::endl;
}

void SPIClass::end()
{
    std::cout << "SPI end called" << std::endl;
}

void SPIClass::setHwCs(bool use)
{
    std::cout << "Set hardware CS to: " << (use ? "true" : "false") << std::endl;
}

uint8_t SPIClass::transfer(uint8_t data)
{
    std::cout << "SPI transfer called with data: " << (int)data << std::endl;
    return 0; // Mock return value
}