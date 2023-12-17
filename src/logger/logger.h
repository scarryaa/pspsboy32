#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

class Logger
{
public:
    void print(const std::string &message);
    void println(const std::string &message);
    void println(uint8_t value);
    static std::string formatLogMessage(uint8_t A, uint8_t F, uint8_t B, uint8_t C, uint8_t D, uint8_t E,
                                        uint8_t H, uint8_t L, uint16_t SP, uint16_t PC,
                                        uint8_t mem0, uint8_t mem1, uint8_t mem2, uint8_t mem3);
};

#endif // __LOGGER_H__