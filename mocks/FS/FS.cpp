#include "FS.h"

void SerialMock::println(const char *str)
{
    lastPrint = str;
    std::cout << str << std::endl; // Added newline character to simulate println
}

void SerialMock::print(long long value)
{
    std::cout << value;
}

void SerialMock::write(uint8_t value)
{
    std::cout << value;
}

void SerialMock::begin(int baudrate)
{
    std::cout << "Serial.begin(" << baudrate << ")" << std::endl;
}

SerialMock Serial; // Global Serial object definition
