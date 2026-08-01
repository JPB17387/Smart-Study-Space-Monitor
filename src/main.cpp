#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "timer.h"
#include "button.h"
#include "buzzer.h"
#include "config.h"

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
    updateTimer();

    bool motion = isMotionDetected();
    int light = getLightPercent();
    unsigned long elapsed = getElapsedSeconds();

    ButtonEvent button = updateButton();

    switch (button)
    {
    case BUTTON_UP:
        Serial.println("UP");
        beep(30);
        break;

    case BUTTON_DOWN:
        Serial.println("DOWN");
        beep(30);
        break;

    case BUTTON_SELECT:
        Serial.println("SELECT");
        beep(80);
        break;

    default:
        break;
    }
}