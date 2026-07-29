#include <Arduino.h>

#include "ui.h"
#include "pir.h"
#include "ldr.h"
#include "button.h"
#include "buzzer.h"

bool lastButtonState = false;

void setup()
{
    Serial.begin(9600);

    initPIR();
    initLDR();
    initButton();
    initBuzzer();
    initUI();
    showBootAnimation();

    Serial.println("Smart Study AI started.");
}

void loop()
{
    updatePIR();

    bool motion = isMotionDetected();
    int light = getLightPercent();

    ButtonEvent buttonEvent = updateButton();

    if (buttonEvent == BUTTON_SHORT_PRESS)
    {
        beep(100);
        Serial.println("Short Press");
    }

    if (buttonEvent == BUTTON_LONG_PRESS)
    {
        beep(250);
        Serial.println("Long Press");
    }

    updateUI(motion, light, buttonEvent);

    delay(100);
}