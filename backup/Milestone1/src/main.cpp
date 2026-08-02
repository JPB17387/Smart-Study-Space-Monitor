#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "button.h"
#include "buzzer.h"

void setup()
{
    Serial.begin(9600);

    // Drivers
    initPIR();
    initLDR();
    initButton();
    initBuzzer();
    initTimer();
    // UI
    initUI();

    // Startup
    runStartupSequence();
}

void loop()
{
    updatePIR();

    int light = getLightPercent();

    bool motion = isMotionDetected();

    updateTimer();

    ButtonEvent button = updateButton();

    updateUI(
        motion,
        light,
        getElapsedSeconds(),
        button
    );
}