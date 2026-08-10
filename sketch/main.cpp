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
 * @brief Coordinates driver updates, session state, recommendation engine, and UI rendering.
 *
 * Sensors feed session state, session state and light level feed recommendation engine,
 * and UI retrieves recommendation without direct session state coupling.
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

}