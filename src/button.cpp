#include <Arduino.h>
#include <config.h>
#include <button.h>

void initButton() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool isButtonPressed(){
    return digitalRead(BUTTON_PIN) == LOW;
}