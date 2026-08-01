#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

//==================================================
// Initialization
//==================================================

void initTimer();
void updateTimer();

//==================================================
// Session Control
//==================================================

void startSession();
void stopSession();
void pauseSession();
void resumeSession();
void resetSession();

//==================================================
// Status
//==================================================

bool isSessionRunning();
bool isSessionPaused();

//==================================================
// Time
//==================================================

unsigned long getElapsedMilliseconds();
unsigned long getElapsedSeconds();

#endif