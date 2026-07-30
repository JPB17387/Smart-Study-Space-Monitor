#include <Arduino.h>

#include "pir.h"
#include "ldr.h"
#include "button.h"
#include "buzzer.h"
#include "ui.h"

//==================================================
// SETUP
//==================================================

void setup()
{
    Serial.begin(9600);

    // Initialize hardware drivers
    initPIR();
    initLDR();
    initButton();
    initBuzzer();


    // Display boot animation
    showBootAnimation();

    // Initialize OLED UI
    initUI();

    Serial.println("==================================");
    Serial.println(" Smart Study AI Platform Started");
    Serial.println("==================================");
}

//==================================================
// MAIN LOOP
//==================================================

void loop()
{
    //--------------------------------------------------
    // Update hardware drivers
    //--------------------------------------------------

    updatePIR();

    //--------------------------------------------------
    // Read sensor values
    //--------------------------------------------------

    bool motion = isMotionDetected();
    int light = getLightPercent();

    //--------------------------------------------------
    // Read button events
    //--------------------------------------------------

    ButtonEvent buttonEvent = updateButton();

    //--------------------------------------------------
    // Optional feedback
    //--------------------------------------------------

    switch (buttonEvent)
    {
        case BUTTON_SHORT_PRESS:
            beep(80);
            Serial.println("Short Press");
            break;

        case BUTTON_LONG_PRESS:
            beep(200);
            Serial.println("Long Press");
            break;

        case BUTTON_NONE:
        default:
            break;
    }

    //--------------------------------------------------
    // Update UI State Machine
    //--------------------------------------------------

    updateUI(
        motion,
        light,
        buttonEvent
    );

    //--------------------------------------------------
    // Temporary refresh rate
    //--------------------------------------------------

    delay(50);
}