#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum ButtonEvent
{
    BUTTON_NONE,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS
};

void initButton();
ButtonEvent updateButton();

#endif