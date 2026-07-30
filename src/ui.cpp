#include "ui.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//==================================================
// OLED Configuration
//==================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

//==================================================
// Private Functions
//==================================================

static void showBootAnimation();
static void showGreeting();
static void showLoadingAnimation();
static void showLogo();

//==================================================
// Initialize OLED
//==================================================

void initUI()
{
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS))
    {
        Serial.println("OLED init failed!");

        while (1);
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);
    display.setTextSize(1);
    display.display();

    Serial.println("OLED initialized successfully.");
}

//==================================================
// Startup Sequence
//==================================================

void runStartupSequence()
{
    showBootAnimation();
    showGreeting();
    showLoadingAnimation();
    showLogo();
}

//==================================================
// Boot Animation
//==================================================

static void showBootAnimation()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(20, 6);
    display.println("SMART STUDY AI");

    display.setCursor(42, 20);
    display.println("BOOT");

    display.drawRect(14, 40, 100, 10, SSD1306_WHITE);

    display.display();

    // Progress bar animation
    for (int progress = 0; progress <= 98; progress += 7)
    {
        display.fillRect(15, 41, progress, 8, SSD1306_WHITE);
        display.display();
        delay(120);
    }

    delay(500);
}

//==================================================
// Greeting Screen
//==================================================

static void showGreeting()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(47, 17);
    display.print("Hi!");

    display.setTextSize(1);
    display.setCursor(23, 39);
    display.print("Sir, Jhon Paul");

    display.display();

    delay(2000);
}

//==================================================
// Loading Animation
//==================================================

static void showLoadingAnimation()
{
    for (int dots = 1; dots <= 9; dots++)
    {
        display.clearDisplay();

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

    display.print(" done");
    display.display();

    delay(1500);
}

//==================================================
// Logo
//==================================================

static void showLogo()
{
    display.clearDisplay();

    display.drawRect(
        17,
        13,
        91,
        43,
        SSD1306_WHITE
    );

    display.setCursor(21, 22);
    display.print("Smart Study AI");

    display.setCursor(43, 38);
    display.print("Platform");

    display.display();

    delay(5000);
}