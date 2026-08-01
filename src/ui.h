#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "button.h"

//==================================================
// Screen States
//==================================================

enum Screen
{
    SCREEN_BOOT,
    SCREEN_GREETING,
    SCREEN_LOADING,
    SCREEN_LOGO,
    SCREEN_DASHBOARD,
    SCREEN_IDLE,
    SCREEN_BREAK,
    SCREEN_MENU,
    SCREEN_AI
};

//==================================================
// Public Functions
//==================================================

void initUI();

void runStartupSequence();

void updateUI(
    bool motion,
    int light,
    unsigned long elapsedSeconds,
    ButtonEvent button
);

void setScreen(Screen screen);

Screen getCurrentScreen();

#endif