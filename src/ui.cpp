#include "ui.h"
#include "config.h"
#include "button.h"
#include "ldr.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

static SessionMode selectedMode = SESSION_FOCUS;

static unsigned long lastMotionTime = 0;
static unsigned long sessionStartTime = 0;

//==================================================
// OLED OBJECT
//==================================================

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

//==================================================
// UI INTERNAL STATE
//==================================================

// The current screen belongs to the UI system.
static Screen currentScreen = SCREEN_GREETING;


// How long the user can remain motionless
// before entering Idle Mode.
static const unsigned long IDLE_TIMEOUT = 10000;

SessionMode currentSession = SESSION_FOCUS;

//==================================================
// INITIALIZE UI
//==================================================

void initUI()
{
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");

        while (1);
    }

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.display();

    currentScreen = SCREEN_GREETING;

    lastMotionTime = millis();
}

//==================================================
// SCREEN CONTROL
//==================================================

void setScreen(Screen screen)
{
    currentScreen = screen;
}

//==================================================
// GREETING
//==================================================

void showGreeting()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setTextWrap(false);

    display.setCursor(47, 17);
    display.print("Hi!");

    display.setTextSize(1);

    display.setCursor(23, 39);
    display.print("Sir, Jhon Paul");

    display.display();

    delay(2000);
}

//==================================================
// BOOT ANIMATION
//==================================================

void showBootAnimation()
{
    for (int frame = 0; frame < 12; frame++)
    {
        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);
        display.setTextWrap(false);

        // Title
        display.setCursor(23, 04);
        display.print("Smart Study AI");

        // Boot label
        display.setCursor(44, 24);
        display.print("Booting");

        // Draw loader frame
        display.drawRect(14, 38, 100, 10, SSD1306_WHITE);

        // Fill loader
        int fillWidth = (frame + 1) * 8;
        if (fillWidth > 98)
        {
            fillWidth = 98;
        }

        display.fillRect(15, 39, fillWidth, 8, SSD1306_WHITE);

        // Small status text
        display.setCursor(20, 54);
        display.print("Initializing...");

        display.display();
        delay(250);
    }
}

//==================================================
// LOADING ANIMATION
//==================================================

void showLoadingAnimation()
{
    for (int dots = 1; dots <= 9; dots++)
    {
        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);
        display.setTextSize(1);
        display.setTextWrap(false);

        display.setCursor(4, 5);
        display.print("Hello Sir Jhon Paul");

        display.setCursor(2, 28);
        display.print("WELCOME BACK ON TRACK");

        display.setCursor(4, 52);
        display.print("Loading");

        for (int i = 0; i < dots; i++)
        {
            display.print(".");
        }

        display.display();

        delay(200);
    }

    delay(1000);
}

//==================================================
// LOGO
//==================================================

void showLogo()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    display.drawRect(
        1,
        0,
        127,
        63,
        SSD1306_WHITE
    );

    display.setCursor(23, 22);
    display.print("Smart Study AI");

    display.setCursor(38, 38);
    display.print("Assistant");

    display.display();

    delay(3000);
}

//==================================================
// MODE MENU DISPLAY
//==================================================

void showModeMenu(SessionMode mode)
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    display.setCursor(0, 0);
    display.println("Session Menu");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print("Select: ");

    if (mode == SESSION_FOCUS)
    {
        display.println("Focus");
    }
    else if (mode == SESSION_BREAK)
    {
        display.println("Break");
    }
    else
    {
        display.println("AI Tips");
    }

    display.setCursor(0, 34);
    display.println("Short press: change");

    display.setCursor(0, 46);
    display.println("Long press: start");

    display.display();
}

//==================================================
// BREAK SESSION SCREEN
//==================================================

void showBreakScreen(unsigned long elapsedSeconds, bool motion, int light)
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    display.setCursor(0, 0);
    display.println("BREAK SESSION");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print("Time : ");
    display.print(elapsedSeconds);
    display.println(" s");

    display.setCursor(0, 30);
    display.print("Motion: ");
    display.println(motion ? "YES" : "NO");

    display.setCursor(0, 42);
    display.print("Light : ");
    display.print(light);
    display.println("%");

    display.setCursor(0, 54);
    display.println("Rest well");

    display.display();
}

//==================================================
// FOCUS SCREEN DASHBOARD
//==================================================

void showFocusScreen(unsigned long elapsedSeconds, bool motion, int light)
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    display.setCursor(0, 0);
    display.println("FOCUS SESSION");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print("Time : ");
    display.print(elapsedSeconds);
    display.println(" s");

    display.setCursor(0, 30);
    display.print("Motion: ");
    display.println(motion ? "YES" : "NO");

    display.setCursor(0, 42);
    display.print("Light : ");
    display.print(light);
    display.println("%");

    display.setCursor(0, 54);
    display.println("Press to exit");

    display.display();
}

//==================================================
// DASHBOARD
//==================================================

void showDashboard(bool motion, int light, SessionMode selectedMode)
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    // Header
    display.setCursor(0, 0);
    display.println("Smart Study AI");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    // Motion
    display.setCursor(0, 18);
    display.print("Motion: ");

    if (motion)
    {
        display.println("Detected");
    }
    else
    {
        display.println("None");
    }

    // Light
    display.setCursor(0, 32);
    display.print("Light : ");
    display.print(light);
    display.println("%");

    // Status
    display.setCursor(0, 46);
    display.print("Status: ");

    if (motion)
    {
        display.println("Studying");
    }
    else
    {
        display.println("Waiting");
    }

    display.display();
}

//==================================================
// IDLE SCREEN
//==================================================

void showIdleScreen()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("Smart Study AI");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    display.setCursor(0, 20);
    display.println("Idle Mode");

    display.setCursor(0, 35);
    display.println("No Motion");

    display.setCursor(0, 50);
    display.println("Waiting...");

    display.display();
}

//==================================================
// FOR AI GENERATED RECOMMENDATIONS
//==================================================
void showAIRecommendation(bool motion, int light)
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(false);

    display.setCursor(0, 0);
    display.println("AI RECOMMENDATION");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setCursor(0, 18);

    if (light < 30)
    {
        display.println("Room is dark.");
        display.println("Try turning on");
        display.println("a desk lamp.");
    }
    else if (motion)
    {
        display.println("Great lighting.");
        display.println("Keep studying.");
    }
    else
    {
        display.println("No motion detected.");
        display.println("Save energy.");
    }

    display.display();
}

//==================================================
// MAIN UI STATE MACHINE
//==================================================

void updateUI(bool motion, int light, ButtonEvent buttonEvent)
{
    switch (currentScreen)
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
            sessionStartTime = millis();
            lastMotionTime = millis();
            currentScreen = SCREEN_DASHBOARD;
            break;

        case SCREEN_DASHBOARD:
            showDashboard(motion, light, selectedMode);

            if (motion)
            {
                lastMotionTime = millis();
            }

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_MENU;
            }

            if (!motion && (millis() - lastMotionTime >= IDLE_TIMEOUT))
            {
                currentScreen = SCREEN_IDLE;
            }
            break;

        case SCREEN_MENU:
            showModeMenu(selectedMode);

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                if (selectedMode == SESSION_FOCUS)
                {
                    selectedMode = SESSION_BREAK;
                }
                else if (selectedMode == SESSION_BREAK)
                {
                    selectedMode = SESSION_AI;
                }
                else
                {
                    selectedMode = SESSION_FOCUS;
                }
            }

            if (buttonEvent == BUTTON_LONG_PRESS)
            {
                sessionStartTime = millis();

                if (selectedMode == SESSION_FOCUS)
                {
                    currentScreen = SCREEN_FOCUS;
                }
                else if (selectedMode == SESSION_BREAK)
                {
                    currentScreen = SCREEN_BREAK;
                }
                else
                {
                    currentScreen = SCREEN_AI_RECOMMENDATION;
                }
            }
            break;

        case SCREEN_FOCUS:
            showFocusScreen(
                (millis() - sessionStartTime) / 1000,
                motion,
                light
            );

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_DASHBOARD;
            }
            break;

        case SCREEN_BREAK:
            showBreakScreen(
                (millis() - sessionStartTime) / 1000,
                motion,
                light
            );

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_DASHBOARD;
            }
            break;

        case SCREEN_AI_RECOMMENDATION:
            showAIRecommendation(motion, light);

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_DASHBOARD;
            }
            break;

        case SCREEN_IDLE:
            showIdleScreen();

            if (motion)
            {
                lastMotionTime = millis();
                currentScreen = SCREEN_DASHBOARD;
            }
            break;
    }
}