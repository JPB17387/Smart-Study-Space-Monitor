#include "timer.h"

//==================================================
// Private Variables
//==================================================

static unsigned long sessionStartTime = 0;
static unsigned long pausedTime = 0;

static bool running = false;
static bool paused = false;

//==================================================
// Initialization
//==================================================

void initTimer()
{
    sessionStartTime = millis();
    pausedTime = 0;

    running = true;
    paused = false;
}

//==================================================
// Update
//==================================================

void updateTimer()
{
    // Reserved for future countdown/Pomodoro logic.
}

//==================================================
// Session Control
//==================================================

void startSession()
{
    sessionStartTime = millis();
    pausedTime = 0;

    running = true;
    paused = false;
}

void stopSession()
{
    running = false;
    paused = false;
}

void pauseSession()
{
    if (running && !paused)
    {
        pausedTime = millis();
        paused = true;
    }
}

void resumeSession()
{
    if (running && paused)
    {
        sessionStartTime += millis() - pausedTime;
        paused = false;
    }
}

void resetSession()
{
    sessionStartTime = millis();
    pausedTime = 0;
}

//==================================================
// Status
//==================================================

bool isSessionRunning()
{
    return running;
}

bool isSessionPaused()
{
    return paused;
}

//==================================================
// Time
//==================================================

unsigned long getElapsedMilliseconds()
{
    if (!running)
        return 0;

    if (paused)
        return pausedTime - sessionStartTime;

    return millis() - sessionStartTime;
}

unsigned long getElapsedSeconds()
{
    return getElapsedMilliseconds() / 1000;
}