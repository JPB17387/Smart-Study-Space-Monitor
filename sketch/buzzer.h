#ifndef BUZZER_H
#define BUZZER_H

// Command feedback cues -- one pulse pattern per WebUI action.
enum BuzzerCue
{
    BUZZER_CUE_START_FOCUS,
    BUZZER_CUE_PAUSE,
    BUZZER_CUE_RESUME,
    BUZZER_CUE_STOP,
    BUZZER_CUE_START_BREAK
};

void initBuzzer();

// Non-blocking pulse-pattern engine. Call once per loop() iteration.
void updateBuzzer();

// Plays a command-feedback pattern immediately, preempting whatever is
// currently sounding (idle tick or a still-finishing pattern). Safe to
// call from an RPC/command handler: only updates state, never blocks.
void playBuzzerCue(BuzzerCue cue);

// Queues a single short notification pulse (e.g. the recommendation
// text changed). Lower priority than playBuzzerCue(): it waits its
// turn instead of interrupting an in-progress cue.
void queueNotificationBeep();

// Idle reminder: a low-priority repeating pulse. It only sounds while
// the buzzer isn't busy with a cue/notification, and the caller is
// expected to stop it the instant motion is detected (see main.cpp).
void startIdleWarningBuzzer();
void stopIdleWarningBuzzer();
bool isIdleWarningBuzzerActive();

// Legacy blocking helper, kept only for backward compatibility. Do
// NOT call this from an RPC/command handler -- it was the source of
// the silent-buzzer bug (see notes in buzzer.cpp). Prefer
// playBuzzerCue().
void beep(unsigned int durationMs = 100);

#endif
