#include <iostream>

// Mocked esp32-hal-gpio.h
#ifndef ESP32_HAL_GPIO_H
#define ESP32_HAL_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#define OUTPUT 0x03
#define PULLUP 0x04
#define INPUT_PULLUP 0x05
#define PULLDOWN 0x08
#define INPUT_PULLDOWN 0x09
#define OPEN_DRAIN 0x10
#define OUTPUT_OPEN_DRAIN 0x12
#define ANALOG 0xC0

    // Mock function signatures
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t val);
    int digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif // ESP32_HAL_GPIO_H
