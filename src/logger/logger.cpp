#include "logger.h"

std::string Logger::formatLogMessage(
    uint8_t A, uint8_t F, uint8_t B, uint8_t C, uint8_t D, uint8_t E,
    uint8_t H, uint8_t L, uint16_t SP, uint16_t PC,
    uint8_t mem0, uint8_t mem1, uint8_t mem2, uint8_t mem3)
{
    std::ostringstream stream;
    stream << std::hex << std::uppercase << std::setfill('0');
    stream << "A:" << std::setw(2) << static_cast<int>(A)
           << " F:" << std::setw(2) << static_cast<int>(F)
           << " B:" << std::setw(2) << static_cast<int>(B)
           << " C:" << std::setw(2) << static_cast<int>(C)
           << " D:" << std::setw(2) << static_cast<int>(D)
           << " E:" << std::setw(2) << static_cast<int>(E)
           << " H:" << std::setw(2) << static_cast<int>(H)
           << " L:" << std::setw(2) << static_cast<int>(L)
           << " SP:" << std::setw(4) << static_cast<int>(SP)
           << " PC:" << std::setw(4) << static_cast<int>(PC)
           << " PCMEM:" << std::setw(2) << static_cast<int>(mem0) << ","
           << std::setw(2) << static_cast<int>(mem1) << ","
           << std::setw(2) << static_cast<int>(mem2) << ","
           << std::setw(2) << static_cast<int>(mem3);
    return stream.str();
}

#ifdef PLATFORM_ESP32
#include <USBCDC.h>
#include <HardwareSerial.h>

void Logger::print(uint8_t value)
{
    Serial.print(value);
}

void Logger::print(const std::string &message)
{
    Serial.print(message.c_str());
}

void Logger::println(const std::string &message)
{
    Serial.println(message.c_str());
}

void Logger::println(uint8_t value)
{
    Serial.println(value);
}

#else
#include <iostream>

void Logger::print(uint8_t value)
{
    std::cout << value;
}

void Logger::print(const std::string &message)
{
    std::cout << message;
}

void Logger::println(const std::string &message)
{
    std::cout << message << std::endl;
}

void Logger::println(uint8_t value)
{
    std::cout << static_cast<int>(value) << std::endl;
}

#endif
