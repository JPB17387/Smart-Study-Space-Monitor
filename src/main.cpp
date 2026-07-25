#include <Arduino.h>
#include "ui.h"
#include "pir.h"
#include "ldr.h"

Screen currentScreen = SCREEN_GREETING;

unsigned long lastMotionTime = 0;

const unsigned long IDLE_TIMEOUT = 10000;

bool lastMotion = false;
int lastLight = -1;

void setup()
{
    Serial.begin(9600);

    initLDR();
    initPIR();
    initUI();
}

void loop()
{
    bool motion = isMotionDetected();

    int light = getLightPercent();

    updateUI(motion, light);

    delay(100);
}