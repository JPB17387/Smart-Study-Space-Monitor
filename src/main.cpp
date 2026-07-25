#include <Arduino.h>
#include "ui.h"
#include "pir.h"

void setup()
{
    initDisplay();

    initPIR();
}

void loop()
{
    updatePIR();

    updateUI();
}