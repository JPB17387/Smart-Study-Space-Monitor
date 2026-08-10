#include <Arduino.h>

#include "config.h"
#include "session.h"
#include "timer.h"

//==================================================
// Private Variables
//==================================================

static SessionState currentSessionState = SESSION_FOCUS;

/**
 * @brief Applies a session-state transition once and updates timer state.
 *
 * The session state owns timer pause/resume decisions so the UI and main
 * coordinator do not duplicate transition logic. BREAK remains reserved.
 *
 * @param newState State requested by the PIR-based session rules.
 */
static void setSessionState(SessionState newState)
{
    if (currentSessionState == newState)
    {
        return;
    }

    currentSessionState = newState;

    switch (currentSessionState)
    {
        case SESSION_FOCUS:
            resumeSession();
            Serial.println(F("Session -> FOCUS"));
            break;

        case SESSION_IDLE:
            pauseSession();
            Serial.println(F("Session -> IDLE"));
            break;

        case SESSION_BREAK:
            Serial.println(F("Session -> BREAK"));
            break;
    }
}

/**
 * @brief Initializes the session state without resetting the study timer.
 *
 * The existing timer starts in initTimer(). Focus is retained as the initial
 * state so elapsed time continues until the configured idle timeout expires.
 */
void initSession()
{
    currentSessionState = SESSION_FOCUS;
}

/**
 * @brief Determines the session state from existing PIR data.
 *
 * Motion resumes Focus immediately. Continuous inactivity reaching
 * SESSION_IDLE_TIMEOUT pauses the existing timer. BREAK has no automatic
 * transition in this phase.
 *
 * @param motion Current motion state from the PIR module.
 * @param idleSeconds Continuous inactivity from the PIR module.
 */
void updateSession(
    bool motion,
    unsigned long idleSeconds)
{
    if (motion)
    {
        setSessionState(SESSION_FOCUS);
        return;
    }

    if (idleSeconds >= SESSION_IDLE_TIMEOUT)
    {
        setSessionState(SESSION_IDLE);
    }
}

/**
 * @brief Returns the current study session state for UI presentation.
 *
 * @return The single authoritative session state.
 */
SessionState getSessionState()
{
    return currentSessionState;
}

void startFocusSession()
{
    currentSessionState = SESSION_FOCUS;
    startSession();
}

void pauseFocusSession()
{
    currentSessionState = SESSION_IDLE;
    pauseSession();
}

void resumeFocusSession()
{
    currentSessionState = SESSION_FOCUS;
    resumeSession();
}

void stopFocusSession()
{
    currentSessionState = SESSION_IDLE;
    stopSession();
}

void startBreakSession()
{
    currentSessionState = SESSION_BREAK;
    stopSession();
}