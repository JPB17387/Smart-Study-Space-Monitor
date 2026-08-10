#include <Arduino.h>
#include "config.h"
#include "ldr.h"


void initLDR()
{
    pinMode(LDR_PIN, INPUT);
}

int readLDR()
{
    return analogRead(LDR_PIN);
}

int getLightPercent()
{
    int value = readLDR();

    // Map 0-1023 to 100-0 because most LDR modules
    // output lower values when brighter.
    int percent = map(value, 0, 1023, 100, 0);

    percent = constrain(percent, 0, 100);

    return percent;
}

const char* getLightStatus()
{
    int light = getLightPercent();

    if(light >= 80)
        return "Very Bright";

    if(light >= 60)
        return "Bright";

    if(light >= 40)
        return "Normal";

    if(light >= 20)
        return "Dim";

    return "Dark";
}