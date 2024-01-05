#include "core.h"

Core::Core() : cpu(memory, timer), ppu(memory), timer(memory)
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

void Core::requestJoypadInterrupt()
{
    memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x10);
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

void Core::setButtonState(Button button, bool pressed)
{
    if (!pressed)
    {
        // Set button state to 0
        switch (button)
        {
        case Button::A:
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0xFE);
            break;
        case Button::B:
            printf("B\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x02);
            break;
        case Button::Select:
            printf("Select\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x04);
            break;
        case Button::Start:
            printf("Start\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x08);
            break;
        case Button::Right:
            printf("Right\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x10);
            break;
        case Button::Left:
            printf("Left\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x20);
            break;
        case Button::Up:
            printf("Up\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x40);
            break;
        case Button::Down:
            printf("Down\n");
            memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x80);
            break;
        }

        // Set bit 5 of P1 to 0
        memory.writeByte(0xFF00, memory.readByte(0xFF00) & 0xDF);

        // Set bit 4 of P1 to 1
        memory.writeByte(0xFF00, memory.readByte(0xFF00) | 0x10);

        // Request joypad interrupt
        memory.writeByte(0xFF0F, memory.readByte(0xFF0F) | 0x10);
    }
    else
    {
    }
}