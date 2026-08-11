#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Arduino_RPClite.h>
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "session.h"
#include "recommendation.h"
#include "buzzer.h"
#include "communication.h"

/**
 * @brief Initializes hardware drivers, timer, session logic, recommendation provider, and UI.
 *
 * The existing startup order is preserved. Session initialization follows
 * timer initialization, followed by recommendation provider initialization.
 */

void ping()
{
    Serial.println(F("RPC ping received"));
}

void setup()
{
    Serial.begin(9600);

    initPIR();
    initLDR();
    initBuzzer();
    initTimer();
    initSession();
    initRecommendation();
    initCommunication();

    Bridge.begin();
    Bridge.provide("ping", ping);
}

/**
 * @brief Coordinates driver updates, session state, recommendation engine, buzzer, and UI.
 *
 * Sensors feed session state, session state and light level feed recommendation engine,
 * UI retrieves recommendation without direct session state coupling, and the buzzer
 * engine is advanced non-blockingly once per iteration.
 */
void loop() { 

    updateCommunication();
  
    updatePIR();

    int light = getLightPercent();

    bool motion = isMotionDetected();

    updateTimer();

    updateSession(
        motion,
        getIdleSeconds()
    );

    updateRecommendation(
        getSessionState(),
        light
    );

    // Idle reminder reuses the existing SESSION_IDLE transition (driven
    // by SESSION_IDLE_TIMEOUT in config.h) rather than a second,
    // unrelated timeout system. Motion cancels it immediately: as soon
    // as the PIR sees motion, updateSession() above moves the state
    // back to SESSION_FOCUS, and this stops the reminder on the very
    // next loop iteration -- it never touches the rest of the session.
    if (getSessionState() == SESSION_IDLE)
    {
        startIdleWarningBuzzer();
    }
    else
    {
        stopIdleWarningBuzzer();
    }

    // One notification pulse per actual recommendation change, not per
    // telemetry poll -- hasRecommendationChanged() already tracks this.
    if (hasRecommendationChanged())
    {
        queueNotificationBeep();
    }

    updateBuzzer();

}
