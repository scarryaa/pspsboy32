#include "core.h"

Core::Core() : cpu(memory, timer), ppu(memory), timer(memory), joypadState(0xFF), memory(cartridge)
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

uint8_t *Core::getDebugFrameBuffer()
{
    return ppu.getDebugFrameBuffer();
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
void Core::loadRom(const std::string &path)
{
    // Load ROM implementation
    cartridge.loadRom(path);
}

void Core::setButtonState(Button button, bool pressed)
{
    // read bits 4-7 of P1 to see if we are selecting buttons or directions
    uint8_t p1 = memory.readByte(0xFF00);
    bool selectingButtons = (p1 & 0x20) == 0;
    bool selectingDirections = (p1 & 0x10) == 0;

    if (selectingButtons)
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
        }

        // set bit 5 to 0 to indicate that a button is pressed
        joypadState &= ~(1 << 5);

        // set bit 4 to 1 to indicate that a button is pressed
        joypadState |= (1 << 4);
    }
    else if (selectingDirections)
    {
        switch (button)
        {
        case Button::Right:
            if (pressed)
                joypadState &= ~(1 << 0); // Clear bit 0 for Right
            else
                joypadState |= (1 << 0); // Set bit 0 for Right
            break;

        case Button::Left:
            if (pressed)
                joypadState &= ~(1 << 1); // Clear bit 1 for Left
            else
                joypadState |= (1 << 1); // Set bit 1 for Left
            break;

        case Button::Up:
            if (pressed)
                joypadState &= ~(1 << 2); // Clear bit 2 for Up
            else
                joypadState |= (1 << 2); // Set bit 2 for Up
            break;

        case Button::Down:
            if (pressed)
                joypadState &= ~(1 << 3); // Clear bit 3 for Down
            else
                joypadState |= (1 << 3); // Set bit 3 for Down
            break;
        }

        // set bit 4 to 0 to indicate that a direction key is pressed
        joypadState &= ~(1 << 4);

        // set bit 5 to 1 to indicate that a direction key is pressed
        joypadState |= (1 << 5);
    }

    // write joypadState to P1
    memory.writeByte(0xFF00, joypadState);
}

uint8_t Core::getJoypadState() const
{
    return joypadState;
}