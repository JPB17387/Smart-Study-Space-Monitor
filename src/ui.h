#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "button.h"

//==================================================
// SCREEN STATES
//==================================================

enum Screen
{
    SCREEN_BOOT,
    SCREEN_GREETING,
    SCREEN_LOADING,
    SCREEN_LOGO,
    SCREEN_MENU,
    SCREEN_DASHBOARD,
    SCREEN_AI,
    SCREEN_BREAK,
    SCREEN_IDLE
};

//==================================================
// MENU ITEMS
//==================================================

enum MenuItem
{
    MENU_FOCUS,
    MENU_AI,
    MENU_BREAK,
    MENU_COUNT
};

//==================================================
// PUBLIC FUNCTIONS
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