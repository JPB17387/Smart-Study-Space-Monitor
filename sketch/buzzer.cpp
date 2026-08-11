#include <Arduino.h>
#include "config.h"
#include "buzzer.h"

/**
 * Why the buzzer was silent
 * --------------------------
 * Every command RPC (rpcStartSession, rpcPauseSession, ...) is
 * registered with Bridge.provide_safe() and called the old beep(),
 * which does digitalWrite(HIGH) -> delay(ms) -> digitalWrite(LOW)
 * directly inside the RPC callback. "provide_safe" callbacks run in a
 * constrained RPC-dispatch context, not the free-running Arduino
 * loop() -- blocking that context with delay() is exactly the kind of
 * call the "safe" variant exists to guard against, so the pin either
 * never got its HIGH window scheduled or was reset before the pulse
 * was long enough to be audible. The fix is to never block inside an
 * RPC handler: playBuzzerCue() below only sets state (a few
 * variable writes) and returns immediately, and the actual pin
 * toggling happens in updateBuzzer(), driven by the normal loop().
 */

//==================================================
// Pulse pattern definitions
//==================================================
// Each cue is a short sequence of ON/OFF pulses played back using
// millis() timing -- no delay() anywhere in this engine.

namespace
{

struct Pulse
{
    unsigned int onMs;
    unsigned int offMs; // gap AFTER this pulse; ignored on the last pulse
};

struct PatternRef
{
    const Pulse* pulses;
    unsigned int count;
};

const Pulse PATTERN_START_FOCUS[] = { {200, 0} };
const Pulse PATTERN_PAUSE[]       = { {100, 0} };
const Pulse PATTERN_RESUME[]      = { {90, 90}, {90, 0} };
const Pulse PATTERN_STOP[]        = { {250, 0} };
const Pulse PATTERN_START_BREAK[] = { {150, 100}, {150, 0} };
const Pulse PATTERN_NOTIFICATION[] = { {60, 0} };
const Pulse PATTERN_IDLE_TICK[]    = { {70, 0} };

const PatternRef CUE_PATTERNS[] = {
    { PATTERN_START_FOCUS, 1 }, // BUZZER_CUE_START_FOCUS
    { PATTERN_PAUSE,       1 }, // BUZZER_CUE_PAUSE
    { PATTERN_RESUME,      2 }, // BUZZER_CUE_RESUME
    { PATTERN_STOP,        1 }, // BUZZER_CUE_STOP
    { PATTERN_START_BREAK, 2 }, // BUZZER_CUE_START_BREAK
};

const PatternRef NOTIFICATION_PATTERN = { PATTERN_NOTIFICATION, 1 };
const PatternRef IDLE_TICK_PATTERN    = { PATTERN_IDLE_TICK, 1 };

const unsigned long IDLE_TICK_INTERVAL_MS = 2000; // gap between idle reminder pulses

//==================================================
// Engine state
//==================================================

enum EngineState { ENGINE_FREE, ENGINE_PULSE_ON, ENGINE_PULSE_GAP };

EngineState engineState = ENGINE_FREE;
const PatternRef* activePattern = nullptr;
unsigned int activePulseIndex = 0;
unsigned long stateChangeAt = 0;

bool idleWarningActive = false;
unsigned long lastIdleTickAt = 0;

bool notificationPending = false;

//==================================================
// Low-level pin drive
//==================================================

void buzzerOn()
{
    digitalWrite(BUZZER_PIN, HIGH);
}

void buzzerOff()
{
    digitalWrite(BUZZER_PIN, LOW);
}

void beginPattern(const PatternRef& pattern)
{
    activePattern = &pattern;
    activePulseIndex = 0;
    buzzerOn();
    engineState = ENGINE_PULSE_ON;
    stateChangeAt = millis();
}

} // namespace

//==================================================
// Public API
//==================================================

void initBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
    buzzerOff();
}

void playBuzzerCue(BuzzerCue cue)
{
    // Highest priority: preempt an idle tick or a still-finishing
    // pattern immediately. Only touches state -- safe to call from an
    // RPC handler.
    beginPattern(CUE_PATTERNS[cue]);
}

void queueNotificationBeep()
{
    notificationPending = true;
}

void startIdleWarningBuzzer()
{
    if (idleWarningActive)
    {
        return;
    }

    idleWarningActive = true;
    // Fire the first reminder almost immediately rather than waiting
    // a full interval after the threshold is crossed.
    lastIdleTickAt = millis() - IDLE_TICK_INTERVAL_MS;
}

void stopIdleWarningBuzzer()
{
    idleWarningActive = false;
}

bool isIdleWarningBuzzerActive()
{
    return idleWarningActive;
}

void updateBuzzer()
{
    unsigned long now = millis();

    // Advance any in-progress pulse (cue, notification, or idle tick).
    if (engineState == ENGINE_PULSE_ON)
    {
        const Pulse& p = activePattern->pulses[activePulseIndex];

        if (now - stateChangeAt >= p.onMs)
        {
            buzzerOff();
            stateChangeAt = now;

            if (p.offMs > 0 && (activePulseIndex + 1) < activePattern->count)
            {
                engineState = ENGINE_PULSE_GAP;
            }
            else
            {
                engineState = ENGINE_FREE;
                activePattern = nullptr;
            }
        }
    }
    else if (engineState == ENGINE_PULSE_GAP)
    {
        const Pulse& p = activePattern->pulses[activePulseIndex];

        if (now - stateChangeAt >= p.offMs)
        {
            activePulseIndex++;
            buzzerOn();
            engineState = ENGINE_PULSE_ON;
            stateChangeAt = now;
        }
    }

    if (engineState != ENGINE_FREE)
    {
        return;
    }

    // Priority 2: a pending notification plays as soon as the buzzer
    // is free, without cutting off a command cue.
    if (notificationPending)
    {
        notificationPending = false;
        beginPattern(NOTIFICATION_PATTERN);
        return;
    }

    // Priority 3: idle reminder, lowest priority -- only ticks when
    // nothing else is queued.
    if (idleWarningActive && (now - lastIdleTickAt >= IDLE_TICK_INTERVAL_MS))
    {
        lastIdleTickAt = now;
        beginPattern(IDLE_TICK_PATTERN);
    }
}

void beep(unsigned int durationMs)
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(durationMs);
    digitalWrite(BUZZER_PIN, LOW);
}
