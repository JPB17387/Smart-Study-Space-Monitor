#include <Arduino.h>

#include "display.h"
#include "ui.h"
#include "pir.h"

void setup() {
    Serial.begin(9600);

    initDisplay();

    initPIR();

    showGreeting();
    showLoadingAnimation();
    showLogo(); 
}

void loop()
{
    bool motion = isMotionDetected();

    showDashboard(motion);

    delay(100);
}