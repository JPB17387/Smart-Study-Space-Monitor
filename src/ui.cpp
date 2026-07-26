#include "ui.h"
#include "config.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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

// Time when motion was last detected.
static unsigned long lastMotionTime = 0;

// How long the user can remain motionless
// before entering Idle Mode.
static const unsigned long IDLE_TIMEOUT = 10000;

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
// DASHBOARD
//==================================================

void showDashboard(bool motion, int light)
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
// BREAK SCREEN
//==================================================

void showBreakScreen()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("Break Time");

    display.display();
}

//==================================================
// MAIN UI STATE MACHINE
//==================================================

void updateUI(bool motion, int light)
{
    switch (currentScreen)
    {
        //--------------------------------------------------
        // CALIBRATION
        //--------------------------------------------------

        case SCREEN_CALIBRATION:

            break;


        //--------------------------------------------------
        // GREETING
        //--------------------------------------------------

        case SCREEN_GREETING:

            showGreeting();

            currentScreen = SCREEN_LOADING;

            break;


        //--------------------------------------------------
        // LOADING
        //--------------------------------------------------

        case SCREEN_LOADING:

            showLoadingAnimation();

            currentScreen = SCREEN_LOGO;

            break;


        //--------------------------------------------------
        // LOGO
        //--------------------------------------------------

        case SCREEN_LOGO:

            showLogo();

            // Start the idle timer when dashboard begins.
            lastMotionTime = millis();

            currentScreen = SCREEN_DASHBOARD;

            break;


        //--------------------------------------------------
        // DASHBOARD
        //--------------------------------------------------

        case SCREEN_DASHBOARD:

            showDashboard(motion, light);

            // Motion means the user is active.
            if (motion)
            {
                lastMotionTime = millis();
            }

            // Only enter Idle Mode after 10 seconds
            // without detecting motion.
            if (!motion &&
                (millis() - lastMotionTime >= IDLE_TIMEOUT))
            {
                currentScreen = SCREEN_IDLE;
            }

            break;


        //--------------------------------------------------
        // IDLE
        //--------------------------------------------------

        case SCREEN_IDLE:

            showIdleScreen();

            // If the user starts moving again,
            // return to the dashboard.
            if (motion)
            {
                lastMotionTime = millis();

                currentScreen = SCREEN_DASHBOARD;
            }

            break;


        //--------------------------------------------------
        // BREAK
        //--------------------------------------------------

        case SCREEN_BREAK:

            showBreakScreen();

            break;
    }
}