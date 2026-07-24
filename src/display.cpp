#include "display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED object
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

void initDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        while (1);
    }

    display.clearDisplay();
    display.display();
}