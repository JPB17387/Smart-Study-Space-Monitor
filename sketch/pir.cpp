#include <Arduino.h>
#include "config.h"
#include "pir.h"

//==================================================
// Private Variables
//==================================================

static bool motion = false;

static bool previousMotion = false;

static bool calibrated = false;

static unsigned long calibrationStart = 0;

static unsigned long lastMotionTime = 0;

static unsigned long motionCounter = 0;

//==================================================
// Initialize PIR
//==================================================

void initPIR()
{
    pinMode(PIR_PIN, INPUT);

    calibrationStart = millis();

    lastMotionTime = millis();
}

//==================================================
// Update PIR
//==================================================

void updatePIR()
{
    if (!calibrated)
    {
        if (millis() - calibrationStart >= PIR_CALIBRATION_TIME)
        {
            calibrated = true;
        }

        return;
    }

    motion = digitalRead(PIR_PIN);

    if (motion && !previousMotion)
    {
        motionCounter++;

        lastMotionTime = millis();
    }

    previousMotion = motion;
}

//==================================================

bool isMotionDetected()
{
    return motion;
}

//==================================================

bool isPIRCalibrated()
{
    return calibrated;
}

//==================================================

unsigned long getMotionCount()
{
    return motionCounter;
}

//==================================================

unsigned long getIdleSeconds()
{
    return (millis() - lastMotionTime) / 1000;
}