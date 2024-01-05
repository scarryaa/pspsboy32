#include "input.h"

Input::Input()
{
    buttons = 0;
    buttonsPressed = 0;
}

void Input::init()
{
    buttons = 0;
    buttonsPressed = 0;
}

void Input::update()
{
    buttonsPressed = buttons;
}

void Input::setButtonState(Button button, bool pressed)
{
    if (pressed)
    {
        buttons |= (uint8_t)button;
    }
    else
    {
        buttons &= ~(uint8_t)button;
    }
}

uint8_t Input::getButtonState()
{
    // Return the buttons that are pressed
    return buttonsPressed;
}