#include <Arduino.h>

#include "ui.h"
#include "pir.h"
#include "ldr.h"

//==================================================
// SETUP
//==================================================

void setup()
{
    Serial.begin(9600);

    // Initialize sensors
    initPIR();
    initLDR();

    // Initialize OLED and UI
    initUI();

    Serial.println("Smart Study AI started.");
}

//==================================================
// MAIN LOOP
//==================================================

void loop()
{
    // Update the PIR driver's internal state
    updatePIR();

    // Read current sensor data
    bool motion = isMotionDetected();

    int light = getLightPercent();

    // Give sensor data to the UI
    updateUI(motion, light);

    delay(100);
}