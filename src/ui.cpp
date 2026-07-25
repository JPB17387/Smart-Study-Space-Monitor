#include "ui.h"
#include "pir.h"

#include <Arduino.h>

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

void showGreeting() {
  display.clearDisplay();
    // string 3
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(47, 17);
    display.print("Hi!");
    // string 3
    display.setTextSize(1);
    display.setCursor(41, 29);
    display.print("");
    // string 3
    display.setCursor(23, 39);
    display.print("Sir, Jhon Paul");
    display.display();
  delay(2000);
  }

void showLoadingAnimation() {

  for (int dots = 1; dots <= 9; dots++) {

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setCursor(4, 5);
    display.print("Hello Sir Jhon Paul");

    display.setCursor(2, 28);
    display.print("WELCOME BACK ON TRACK");

    display.setCursor(4, 52);
    display.print("Loading");

    for (int i = 0; i < dots; i++) {
      display.print(".");
    }

    display.display();
    delay(200);
  }

  display.print(" done");
  display.display();

  delay(1500);
}

void showLogo() {
  display.clearDisplay();
    // rect 1
    display.drawRect(17, 13, 91, 43, 1);
    // string 2
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(21, 22);
    display.print("Smart Study AI ");
    // string 3
    display.setCursor(38, 38);
    display.print("Assistant");
    display.display();
    delay(5000);
}

void showDashboard()
{
    bool motion = isMotionDetected();

    display.clearDisplay();

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(0,0);
    display.println("Smart Study AI");

    display.drawLine(0,10,128,10,SSD1306_WHITE);

    display.setCursor(0,18);
    display.print("Motion: ");

    if(motion)
        display.println("Detected");
    else
        display.println("None");

    display.setCursor(0,35);
    display.print("Status: ");

    if(motion)
        display.println("Studying");
    else
        display.println("Idle");

    display.display();
}

void showBreakScreen() {
    display.clearDisplay();

    display.setCursor(0,0);
    display.println("Break Time");

    display.display();
}

void showIdleScreen() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Smart Study AI");

    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    display.setCursor(0, 20);
    display.println("Idle Mode");

    display.setCursor(0, 35);
    display.println("No Motion");

    display.display();
}


static ScreenType currentScreen = SCREEN_GREETING;

void setScreen(ScreenType screen)
{
    currentScreen = screen;
}

void updateUI()
{
    switch(currentScreen)
    {
        case SCREEN_CALIBRATION:
            break;

        case SCREEN_GREETING:
            showGreeting();
            currentScreen = SCREEN_LOADING;
            break;

        case SCREEN_LOADING:
            showLoadingAnimation();
            currentScreen = SCREEN_LOGO;
            break;

        case SCREEN_LOGO:
            showLogo();
            currentScreen = SCREEN_DASHBOARD;
            break;

        case SCREEN_DASHBOARD:
            showDashboard();

            if(!isMotionDetected())
            {
                currentScreen = SCREEN_IDLE;
            }

            break;

        case SCREEN_IDLE:
            showIdleScreen();

            if(isMotionDetected())
            {
                currentScreen = SCREEN_DASHBOARD;
            }

            break;

        case SCREEN_BREAK:
            showBreakScreen();
            break;
    }
}