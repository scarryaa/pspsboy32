#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../memory/memory.h"

#define TIMER_DIV_DEFAULT 0xABCC
#define TIMA_ADDRESS 0xFF05
#define IF_ADDRESS 0xFF0F

// Timer registers
static uint16_t DIV;
static uint8_t TIMA;
static uint8_t TMA;
static uint8_t TAC;

// Timer counters
static uint32_t divCounter;
static uint32_t timerCounter;

class Timer
{
public:
    Timer(Memory &memory);
    void updateTimers(uint16_t cycles);

private:
    Memory &memory;
};

#endif // TIMER_H