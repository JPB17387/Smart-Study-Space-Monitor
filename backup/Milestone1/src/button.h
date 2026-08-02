#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

//==================================================
// Button Events
//==================================================

enum ButtonEvent
{
    BUTTON_NONE,

    BUTTON_UP,

    BUTTON_DOWN,

    BUTTON_SELECT
};

//==================================================
// Public API
//==================================================

void initButton();

ButtonEvent updateButton();

#endif