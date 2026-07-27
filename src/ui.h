#ifndef UI_H
#define UI_H

#include <Arduino.h>

//==================================================
// SCREEN STATES
//==================================================

enum Screen
{
    SCREEN_CALIBRATION,
    SCREEN_GREETING,
    SCREEN_LOADING,
    SCREEN_LOGO,
    SCREEN_DASHBOARD,
    SCREEN_IDLE,
    SCREEN_BREAK
};

//==================================================
// INITIALIZATION
//==================================================

void initUI();

//==================================================
// UI STATE MACHINE
//==================================================

void updateUI(bool motion, int light);

//==================================================
// INDIVIDUAL SCREENS
//==================================================

void showBootAnimation();
void showGreeting();
void showLoadingAnimation();
void showLogo();

void showDashboard(
    bool motion,
    int light
);

void showIdleScreen();
void showBreakScreen();

//==================================================
// SCREEN CONTROL
//==================================================

void setScreen(Screen screen);

#endif