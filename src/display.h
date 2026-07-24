#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

// Make the OLED object accessible everywhere
extern Adafruit_SSD1306 display;

// Initialize the OLED
void initDisplay();

#endif