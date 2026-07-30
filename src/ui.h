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
    SCREEN_DASHBOARD,
    SCREEN_MENU,
    SCREEN_FOCUS,
    SCREEN_BREAK,
    SCREEN_AI_RECOMMENDATION,
    SCREEN_IDLE
};

//==================================================
// SESSION MODES
//==================================================

enum SessionMode
{
    SESSION_FOCUS,
    SESSION_BREAK,
    SESSION_AI
};

// Current active session
extern SessionMode currentSession;

//==================================================
// INITIALIZATION
//==================================================

void initUI();

//==================================================
// MAIN UI STATE MACHINE
//==================================================

void updateUI(
    bool motion,
    int light,
    ButtonEvent buttonEvent
);

//==================================================
// SCREEN CONTROL
//==================================================

void setScreen(Screen screen);

//==================================================
// STARTUP SCREENS
//==================================================

void showBootAnimation();
void showGreeting();
void showLoadingAnimation();
void showLogo();

//==================================================
// MAIN SCREENS
//==================================================

void showDashboard(
    bool motion,
    int light,
    SessionMode mode
);

void showMenu(
    SessionMode mode
);

void showFocusScreen(
    unsigned long elapsedSeconds,
    bool motion,
    int light
);

void showBreakScreen(
    unsigned long elapsedSeconds,
    bool motion,
    int light
);

void showAIRecommendation(
    bool motion,
    int light
);

void showIdleScreen();

#endif