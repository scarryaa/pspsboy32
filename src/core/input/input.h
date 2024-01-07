#ifndef __INPUT_H__
#define __INPUT_H__

#include <cstdint>
#include <stdio.h>
#include "../memory/memory.h"

enum class Button
{
    A,
    B,
    Select,
    Start,
    Right,
    Left,
    Up,
    Down
};

class Input
{
public:
    Input(Memory &memory);
    ~Input() = default;

    void init();
    void update(uint8_t joypad);

    uint8_t getButtonState();
    void setButtonState(Button button, bool pressed);

private:
    uint8_t buttons;
    uint8_t buttonsPressed;
    Memory &memory;
};

#endif // __INPUT_H__