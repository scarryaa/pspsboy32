#include "core.h"

Core::Core() : cpu(memory), ppu(memory)
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
    int cyclesThisUpdate = 0;

    // CPU executes one instruction, which could take multiple cycles
    cyclesThisUpdate += cpu.executeCycle();

    // Update PPU with the number of cycles that have passed
    ppu.update(cyclesThisUpdate);

    // Update other subsystems if needed
    input.update();
}

uint8_t *Core::getFrameBuffer()
{
    return ppu.getFrameBuffer();
}

bool Core::isFrameReady()
{
    return ppu.isFrameReady();
}

void Core::resetFrameReady()
{
    ppu.resetFrameReady();
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