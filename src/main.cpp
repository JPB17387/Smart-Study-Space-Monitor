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

    Serial.println("Smart Study AI started.");
}

void loop()
{
    updatePIR();

    bool motion = isMotionDetected();
    int light = getLightPercent();

    updateUI(motion, light);

    bool buttonPressed = isButtonPressed();

    if (buttonPressed && !lastButtonState)
    {
        beep(100);
        Serial.println("Button pressed!");
    }

    lastButtonState = buttonPressed;

    delay(100);
}