#ifndef SESSION_H
#define SESSION_H

#include <Arduino.h>

//==================================================
// Session States
//==================================================

enum SessionState : uint8_t
{
    SESSION_IDLE,
    SESSION_FOCUS,
    SESSION_BREAK
};

//==================================================
// Public API
//==================================================

void initSession();

void updateSession(
    bool motion,
    unsigned long idleSeconds
);

SessionState getSessionState();

#endif
