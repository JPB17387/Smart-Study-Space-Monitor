#include "pir.h"

#include <Arduino.h>
// Initialize the PIR sensor pin as an input
void initPIR()
{
    pinMode(PIR_PIN, INPUT);
}
// Returns true if motion is detected 
bool isMotionDetected()
{
    return digitalRead(PIR_PIN) == HIGH;
}