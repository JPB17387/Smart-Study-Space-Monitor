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
    SCREEN_MENU,
    SCREEN_FOCUS,
    SCREEN_BREAK,
    SCREEN_AI_RECOMMENDATION,
    SCREEN_IDLE
};

enum SessionMode
{
    SESSION_FOCUS,
    SESSION_BREAK,
    SESSION_AI
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
    int light,
    SessionMode selectedMode
);
void showFocusScreen(
    unsigned long elapsedSeconds, 
    bool motion, 
    int light
);

void showAIRecommendation(
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