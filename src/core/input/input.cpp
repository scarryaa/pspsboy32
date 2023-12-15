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