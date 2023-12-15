// Mocked esp32-hal-gpio.cpp
#include "esp32-hal-gpio.h"
#include <iostream>

void pinMode(uint8_t pin, uint8_t mode)
{
    // Mock implementation
    std::cout << "Mock pinMode called with pin: " << (int)pin << ", mode: " << (int)mode << std::endl;
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    // Mock implementation
    std::cout << "Mock digitalWrite called with pin: " << (int)pin << ", value: " << (int)val << std::endl;
}

int digitalRead(uint8_t pin)
{
    // Mock implementation
    std::cout << "Mock digitalRead called with pin: " << (int)pin << std::endl;
    return 0; // Example return value
}
