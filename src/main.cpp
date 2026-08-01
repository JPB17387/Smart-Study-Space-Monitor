#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "button.h"
#include "buzzer.h"

void setup()
{
    Serial.begin(115200);

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
    updateTimer();

    bool motion = isMotionDetected();
    int light = getLightPercent();
    unsigned long elapsed = getElapsedSeconds();

    ButtonEvent button = updateButton();

    // Temporary test
    switch (button)
    {
        case BUTTON_UP:
            Serial.println("UP");
            break;

        case BUTTON_DOWN:
            Serial.println("DOWN");
            break;

        case BUTTON_SELECT:
            Serial.println("SELECT");
            break;

        default:
            break;
    }

    updateUI(
        motion,
        light,
        elapsed,
        button
    );
}