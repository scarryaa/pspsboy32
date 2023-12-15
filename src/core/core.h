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
    void loadRom(char *data);

private:
    PPU ppu;
    Memory memory;
    CPU cpu;
    Input input;
};

extern PPU ppu;
extern Memory memory;
extern CPU cpu;
extern Input input;

#endif // CORE_H