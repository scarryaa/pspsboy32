#include <iostream>
#include <string>

class SerialMock
{
public:
    std::string lastPrint;

    void println(const char *str);
    void print(long long value);
    void write(uint8_t value);
    void begin(int baudrate);
};

extern SerialMock Serial;
