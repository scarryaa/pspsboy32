#include "core.h"

Core::Core() : cpu(memory, timer), ppu(memory), timer(memory), joypadState(0xFF)
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

void Core::setButtonState(Button button, bool pressed)
{
    switch (button)
    {
    case Button::A:
        if (pressed)
            joypadState &= ~(1 << 0); // Clear bit 0 for A
        else
            joypadState |= (1 << 0); // Set bit 0 for A
        break;

    case Button::B:
        if (pressed)
            joypadState &= ~(1 << 1); // Clear bit 1 for B
        else
            joypadState |= (1 << 1); // Set bit 1 for B
        break;

    case Button::Select:
        if (pressed)
            joypadState &= ~(1 << 2); // Clear bit 2 for Select
        else
            joypadState |= (1 << 2); // Set bit 2 for Select
        break;

    case Button::Start:
        if (pressed)
            joypadState &= ~(1 << 3); // Clear bit 3 for Start
        else
            joypadState |= (1 << 3); // Set bit 3 for Start
        break;

    case Button::Right:
        if (pressed)
            joypadState &= ~(1 << 4); // Clear bit 4 for Right
        else
            joypadState |= (1 << 4); // Set bit 4 for Right
        break;

    case Button::Left:
        if (pressed)
            joypadState &= ~(1 << 5); // Clear bit 5 for Left
        else
            joypadState |= (1 << 5); // Set bit 5 for Left
        break;

    case Button::Up:
        if (pressed)
            joypadState &= ~(1 << 6); // Clear bit 6 for Up
        else
            joypadState |= (1 << 6); // Set bit 6 for Up
        break;

    case Button::Down:
        if (pressed)
            joypadState &= ~(1 << 7); // Clear bit 7 for Down
        else
            joypadState |= (1 << 7); // Set bit 7 for Down
        break;
    }

    // Update the joypad state in memory
    memory.writeByte(0xFF00, joypadState);
}

uint8_t Core::getJoypadState() const
{
    return joypadState;
}