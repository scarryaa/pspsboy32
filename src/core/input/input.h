#ifndef __INPUT_H__
#define __INPUT_H__

#include <cstdint>

class Input
{
public:
    Input();
    ~Input() = default;

    void init();
    void update();

    void setButton(uint8_t button, bool pressed);

private:
    uint8_t buttons;
    uint8_t buttonsPressed;
};

#endif // __INPUT_H__