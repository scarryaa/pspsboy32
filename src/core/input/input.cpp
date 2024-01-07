#include "input.h"

Input::Input(Memory &memory) : memory(memory)
{
    buttons = 0;
    buttonsPressed = 0;
}

void Input::init()
{
    buttons = 0;
    buttonsPressed = 0;
}

void Input::update(uint8_t joypad)
{
}

void Input::setButtonState(Button button, bool pressed)
{
}

uint8_t Input::getButtonState()
{
    // Return the buttons that are pressed
    return buttonsPressed;
}