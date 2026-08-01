#include "button.h"
#include "config.h"

static bool lastUp = HIGH;
static bool lastDown = HIGH;
static bool lastSelect = HIGH;

static unsigned long lastDebounceUp = 0;
static unsigned long lastDebounceDown = 0;
static unsigned long lastDebounceSelect = 0;

const unsigned long DEBOUNCE_TIME = 40;

//==================================================
// Helper
//==================================================

static bool buttonPressed(
    uint8_t pin,
    bool &lastState,
    unsigned long &lastDebounce)
{
    bool reading = digitalRead(pin);

    if (reading != lastState)
    {
        lastDebounce = millis();
    }

    if ((millis() - lastDebounce) > DEBOUNCE_TIME)
    {
        if (lastState == HIGH && reading == LOW)
        {
            lastState = reading;
            return true;
        }
    }

    lastState = reading;
    return false;
}

//==================================================

void initButton()
{
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
}

//==================================================

ButtonEvent updateButton()
{
    if (buttonPressed(
            BUTTON_UP_PIN,
            lastUp,
            lastDebounceUp))
    {
        return BUTTON_UP;
    }

    if (buttonPressed(
            BUTTON_DOWN_PIN,
            lastDown,
            lastDebounceDown))
    {
        return BUTTON_DOWN;
    }

    if (buttonPressed(
            BUTTON_SELECT_PIN,
            lastSelect,
            lastDebounceSelect))
    {
        return BUTTON_SELECT;
    }

    return BUTTON_NONE;
}

