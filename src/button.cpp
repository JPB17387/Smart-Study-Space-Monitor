#include <Arduino.h>
#include "config.h"
#include "button.h"

static bool lastReading = HIGH;
static bool stableState = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long pressStartTime = 0;
static bool longPressHandled = false;

static const unsigned long DEBOUNCE_MS = 50;
static const unsigned long LONG_PRESS_MS = 700;

void initButton()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

ButtonEvent updateButton()
{
    bool reading = digitalRead(BUTTON_PIN);

    if (reading != lastReading)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_MS)
    {
        if (reading != stableState)
        {
            stableState = reading;

            if (stableState == LOW)
            {
                pressStartTime = millis();
                longPressHandled = false;
            }
            else
            {
                if (!longPressHandled)
                {
                    lastReading = reading;
                    return BUTTON_SHORT_PRESS;
                }
            }
        }

        if (stableState == LOW &&
            !longPressHandled &&
            (millis() - pressStartTime) >= LONG_PRESS_MS)
        {
            longPressHandled = true;
            lastReading = reading;
            return BUTTON_LONG_PRESS;
        }
    }

    lastReading = reading;
    return BUTTON_NONE;
}