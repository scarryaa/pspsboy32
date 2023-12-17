#ifndef CORE_H
#define CORE_H

#include "./cpu/cpu.h"
#include "./ppu/ppu.h"
#include "./memory/memory.h"
#include "./input/input.h"

struct Core
{
public:
    Core();
    ~Core();

    void init();
    void update();
    void render();
    void shutdown();

    void reset();
    void loadRom(char *data, size_t size);

private:
    PPU ppu;
    Memory memory;
    CPU cpu;
    Input input;
};

#endif // CORE_H