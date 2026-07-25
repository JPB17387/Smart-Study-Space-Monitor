#ifndef PIR_H
#define PIR_H

void initPIR();

void updatePIR();

bool isMotionDetected();

bool isPIRCalibrated();

unsigned long getMotionCount();

unsigned long getIdleSeconds();

#endif