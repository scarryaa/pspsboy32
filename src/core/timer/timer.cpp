#include "timer.h"

Timer::Timer(Memory &memory) : memory(memory)
{
    DIV = TIMER_DIV_DEFAULT;
    TIMA = 0;
    TMA = 0;
    TAC = 0;
}

void Timer::updateTimers(uint16_t cycles)
{
    // Update divider register
    divCounter += cycles;
    if (divCounter >= 256)
    {
        memory.writeByte(0xFF04, rand());
        divCounter -= 256;
    }

    // Update timer register
    uint8_t timerControl = memory.readByte(0xFF07);
    if (timerControl & 0x04)
    {
        timerCounter += cycles * 4;
        uint16_t timerFrequency = 4096;
        switch (timerControl & 0x03)
        {
        case 0x00:
            timerFrequency = 1024;
            break;
        case 0x01:
            timerFrequency = 16;
            break;
        case 0x02:
            timerFrequency = 64;
            break;
        case 0x03:
            timerFrequency = 256;
            break;
        }

        if (timerCounter >= timerFrequency)
        {
            uint8_t timerValue = memory.readByte(0xFF05);
            timerValue++;
            if (timerValue == 0)
            {
                memory.writeByte(0xFF05, memory.readByte(0xFF06));        // Reload TIMA with TMA
                memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x04); // Set Timer interrupt flag
            }
            else
            {
                memory.writeByte(0xFF05, timerValue);
            }
            timerCounter -= timerFrequency;
        }
    }
}