#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "session.h"
#include "recommendation.h"
#include "button.h"
#include "buzzer.h"

/**
 * @brief Initializes hardware drivers, timer, session logic, recommendation provider, and UI.
 *
 * The existing startup order is preserved. Session initialization follows
 * timer initialization, followed by recommendation provider initialization.
 */
void setup()
{
    Serial.begin(9600);

    initPIR();
    initLDR();
    initButton();
    initBuzzer();
    initTimer();
    initSession();
    initRecommendation();
    initUI();

    runStartupSequence();
}

/**
 * @brief Coordinates driver updates, session state, recommendation engine, and UI rendering.
 *
 * Sensors feed session state, session state and light level feed recommendation engine,
 * and UI retrieves recommendation without direct session state coupling.
 */
void loop()
{ 
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

    ButtonEvent button = updateButton();

    updateUI(
        motion,
        light,
        getElapsedSeconds(),
        button
    );
}
