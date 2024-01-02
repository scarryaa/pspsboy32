#ifndef __INPUT_H__
#define __INPUT_H__

#include <cstdint>

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
    Input();
    ~Input() = default;

    void init();
    void update();

    uint8_t getButtonState();
    void setButtonState(Button button, bool pressed);

private:
    uint8_t buttons;
    uint8_t buttonsPressed;
};

#endif // __INPUT_H__