#include <Arduino.h>
#include "ui.h"

void setup()
{
    Serial.begin(115200);

    initUI();

    runStartupSequence();
}

void loop()
{
    // Nothing yet.
}