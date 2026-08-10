#include <Arduino.h>
#include "config.h"
#include "buzzer.h"

void initBuzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void beep(unsigned int durationMs)
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(durationMs);
    digitalWrite(BUZZER_PIN, LOW);
}