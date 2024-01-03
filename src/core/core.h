#ifndef CORE_H
#define CORE_H

#include "./cpu/cpu.h"
#include "./ppu/ppu.h"
#include "./memory/memory.h"
#include "./input/input.h"
#include "./timer/timer.h"

struct Core
{
public:
    Core();
    ~Core();

    void init();
    void update();
    void shutdown();
    uint8_t *getFrameBuffer();

    bool isFrameReady();
    void resetFrameReady();

    void reset();
    void loadRom(char *data, size_t size);

    uint8_t getButtonState();
    void setButtonState(Button button, bool pressed);

private:
    PPU ppu;
    Memory memory;
    CPU cpu;
    Input input;
    Timer timer;
};

#endif // CORE_H