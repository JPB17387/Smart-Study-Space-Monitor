#include <Arduino.h>

#include "config.h"
#include "button.h"
#include "buzzer.h"

const unsigned long DEBOUNCE_TIME = 40;

struct ButtonState
{
    uint8_t pin;

    bool lastReading;
    bool stableState;

    unsigned long lastChange;
};

static ButtonState upButton =
{
    BUTTON_UP_PIN,
    HIGH,
    HIGH,
    0
};

static ButtonState downButton =
{
    BUTTON_DOWN_PIN,
    HIGH,
    HIGH,
    0
};

static ButtonState selectButton =
{
    BUTTON_SELECT_PIN,
    HIGH,
    HIGH,
    0
};

void initButton()
{
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
}

static bool buttonPressed(ButtonState &button)
{
    bool reading = digitalRead(button.pin);

    if (reading != button.lastReading)
    {
        button.lastChange = millis();
        button.lastReading = reading;
    }

    if ((millis() - button.lastChange) >= DEBOUNCE_TIME)
    {
        if (button.stableState != reading)
        {
            button.stableState = reading;

            if (reading == LOW)
            {
                return true;
            }
        }
    }

    return false;
}

ButtonEvent updateButton()
{
    if (buttonPressed(upButton))
    {
        beep();
        return BUTTON_UP;
    }

    if (buttonPressed(downButton))
    {
        beep();
        return BUTTON_DOWN;
    }

    if (buttonPressed(selectButton))
    {
        beep(250);
        return BUTTON_SELECT;
    }

    return BUTTON_NONE;
}