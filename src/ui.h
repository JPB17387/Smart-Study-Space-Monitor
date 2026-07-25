#ifndef UI_H
#define UI_H
#include <Arduino.h>

void initDisplay();

void updateUI();

void showGreeting();

void showLoadingAnimation();

void showLogo();

void showDashboard();

void showBreakScreen();

void showIdleScreen();

enum ScreenType
{
    SCREEN_GREETING,
    SCREEN_LOADING,
    SCREEN_LOGO,
    SCREEN_DASHBOARD,
    SCREEN_BREAK,
    SCREEN_IDLE,
    SCREEN_CALIBRATION
};

void setScreen(ScreenType screen);
void updateScreen();

#endif