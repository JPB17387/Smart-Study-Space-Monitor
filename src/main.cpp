#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "session.h"
#include "button.h"
#include "buzzer.h"

/**
 * @brief Initializes hardware drivers, timer, session logic, and UI.
 *
 * The existing startup order is preserved. Session initialization follows
 * timer initialization so it can later control pause and resume behavior.
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
    initUI();
}

/**
 * @brief Coordinates driver updates, session state, and UI rendering.
 *
 * PIR data is read once and supplied to the session state machine and UI.
 * No hardware or UI logic is moved into this coordinator.
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

    ButtonEvent button = updateButton();

    updateUI(
        motion,
        light,
        getElapsedSeconds(),
        button
    );
}
