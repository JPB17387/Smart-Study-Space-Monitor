#pragma once

#include <Arduino.h>

//==================================================
// Session State
//==================================================

enum SessionState
{
    SESSION_FOCUS,
    SESSION_BREAK,
    SESSION_IDLE
};

//==================================================
// Session Management
//==================================================

void initSession();

void updateSession(
    bool motionDetected,
    unsigned long idleSeconds
);

SessionState getSessionState();

//==================================================
// Communication / Control
//==================================================

void startFocusSession();

void pauseFocusSession();

void resumeFocusSession();

void stopFocusSession();

void startBreakSession();