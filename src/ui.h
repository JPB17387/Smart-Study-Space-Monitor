#ifndef UI_H
#define UI_H

#include <Arduino.h>

// Screen states
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

// Initialize UI
void initUI();

// Main UI state machine
void updateUI(bool motion, int light);

// Individual screens
void showGreeting();
void showLoadingAnimation();
void showLogo();
void showDashboard(bool motion, int light);
void showIdleScreen();
void showBreakScreen();

// Change current screen
static Screen currentScreen = SCREEN_GREETING;

#endif