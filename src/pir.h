#ifndef PIR_H
#define PIR_H

// Digital pin connected to the PIR sensor
#define PIR_PIN 2

// Initialize the PIR sensor
void initPIR();

// Returns true if motion is detected
bool isMotionDetected();

#endif