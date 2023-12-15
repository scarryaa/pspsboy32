#include <iostream>

// mock HardwareSerial
class HardwareSerial
{
public:
    static void begin(int baudrate)
    {
        std::cout << "HardwareSerial.begin(" << baudrate << ")" << std::endl;
    }

    static void println(const char *message)
    {
        std::cout << message << std::endl;
    }

    static void println(int value)
    {
        std::cout << value << std::endl;
    }
};