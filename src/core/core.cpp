#include "core.h"

Core::Core() : cpu(memory)
{
}

Core::~Core()
{
    // Destructor implementation
}

void Core::init()
{
    reset();
}

void Core::update()
{
    cpu.executeCycle();
    ppu.update(1);
    input.update();
}

void Core::render()
{
    ppu.update(1);
}

void Core::shutdown()
{
    // Shutdown implementation
}

void Core::reset()
{
    // Reset implementation
    memory.reset();
    cpu.reset();
    ppu.reset();
}

// loadRom from data
void Core::loadRom(char *data, size_t size)
{
    // Load ROM implementation
    memory.loadRom(data, size);
}