#include "communication.h"

#include "Arduino_RouterBridge.h"

#include "buzzer.h"
#include "ldr.h"
#include "pir.h"
#include "recommendation.h"
#include "session.h"
#include "timer.h"

//==================================================
// Telemetry RPC Functions
//==================================================

namespace
{

bool rpcGetMotion()
{
    return isMotionDetected();
}

int rpcGetLight()
{
    return getLightPercent();
}

bool rpcGetFocus()
{
    return getSessionState() == SESSION_FOCUS;
}

unsigned long rpcGetSessionTime()
{
    return getElapsedSeconds();
}

unsigned long rpcGetIdleTime()
{
    return getIdleSeconds();
}

String rpcGetRecommendation()
{
    return String(getRecommendation());
}

//==================================================
// Command RPC
//==================================================

void rpcStartSession()
{
    startFocusSession();
    beep(200);
}

void rpcPauseSession()
{
    pauseFocusSession();
    beep(100);
}

void rpcResumeSession()
{
    resumeFocusSession();
    beep(100);
}

void rpcStopSession()
{
    stopFocusSession();
    beep(250);
}

void rpcStartBreak()
{
    startBreakSession();
    beep(200);
}

} // namespace

//==================================================
// Initialization
//==================================================

void initCommunication()
{
    Bridge.begin();

    // Telemetry
    Bridge.provide(
        "get_motion",
        rpcGetMotion
    );

    Bridge.provide(
        "get_light",
        rpcGetLight
    );

    Bridge.provide(
        "get_focus",
        rpcGetFocus
    );

    Bridge.provide(
        "get_session_time",
        rpcGetSessionTime
    );

    Bridge.provide(
        "get_idle_time",
        rpcGetIdleTime
    );

    Bridge.provide(
        "get_recommendation",
        rpcGetRecommendation
    );

    // Commands
    Bridge.provide_safe(
        "start_session",
        rpcStartSession
    );

    Bridge.provide_safe(
        "pause_session",
        rpcPauseSession
    );

    Bridge.provide_safe(
        "resume_session",
        rpcResumeSession
    );

    Bridge.provide_safe(
        "stop_session",
        rpcStopSession
    );

    Bridge.provide_safe(
        "start_break",
        rpcStartBreak
    );
}

//==================================================
// Update
//==================================================

void updateCommunication()
{
    // RPC handlers are serviced by the Bridge.
}