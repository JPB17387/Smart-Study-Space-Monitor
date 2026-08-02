#ifndef LDR_H
#define LDR_H

// Initialize the LDR module
void initLDR();

// Read the raw analog value (0–1023)
int readLDR();

// Convert the raw value to a percentage (0–100%)
int getLightPercent();

// Return a readable description of the light level
const char* getLightStatus();

#endif