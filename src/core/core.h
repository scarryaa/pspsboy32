#ifndef CORE_H
#define CORE_H

#include "./cpu/cpu.h"
#include "./ppu/ppu.h"
#include "./memory/memory.h"
#include "./timer/timer.h"
#include "./cartridge/cartridge.hpp"

struct Core
{
public:
    typedef enum
    {
        Right = 0,
        Left = 1,
        Up = 2,
        Down = 3,
        A = 4,
        B = 5,
        Select = 6,
        Start = 7
    } Button;

    Core();
    ~Core();

    void init();
    void update();
    void shutdown();
    uint8_t *getFrameBuffer();
    uint8_t *getDebugFrameBuffer();

    bool isFrameReady();
    void resetFrameReady();

    void reset();
    void loadRom(char *data, size_t size);

    uint8_t getButtonState();
    void setButtonState(Button button, bool pressed);
    uint8_t getJoypadState() const;

private:
    PPU ppu;
    Memory memory;
    CPU cpu;
    Timer timer;
    uint8_t joypadState;
    Cartridge cartridge;
};

#endif // CORE_H