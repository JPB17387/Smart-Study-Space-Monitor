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
// UI State
//==================================================

static Screen currentScreen = SCREEN_BOOT;

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
    setScreen(SCREEN_BOOT);

    while (getCurrentScreen() != SCREEN_DASHBOARD)
    {
        updateUI(false, 0, 0, BUTTON_NONE);
    }
}

//==================================================
// Boot Animation
//==================================================

static void showBootAnimation()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(20, 7);
    display.println("SMART STUDY AI");

    display.setCursor(53, 18);
    display.println("BOOT");

    display.setCursor(11, 64);
    display.print("Initializing......");

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

    display.print("done");
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

//==================================================
// Dashboard
//==================================================

static void showDashboard(
    bool motion,
    int light,
    unsigned long elapsedSeconds)
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0,0);
    display.println("Smart Study AI");

    display.drawLine(0,10,127,10,SSD1306_WHITE);

    display.setCursor(0,18);
    display.print("Motion : ");
    display.println(motion ? "YES" : "NO");

    display.setCursor(0,30);
    display.print("Light  : ");
    display.print(light);
    display.println("%");

    display.setCursor(0,42);
    display.print("Mode   : FOCUS");

    unsigned long minutes = elapsedSeconds / 60;
    unsigned long seconds = elapsedSeconds % 60;

    display.setCursor(0,54);
    display.print("Time   : ");

    if(minutes < 10)
        display.print('0');

    display.print(minutes);
    display.print(':');

    if(seconds < 10)
        display.print('0');

    display.print(seconds);

    display.display();
}
//==================================================
// Screen Manager
//==================================================

void setScreen(Screen screen)
{
    currentScreen = screen;
}

Screen getCurrentScreen()
{
    return currentScreen;
}

void updateUI(
    bool motion,
    int light,
    unsigned long elapsedSeconds,
    ButtonEvent button)
    {
        (void)button;

        switch (currentScreen)
        {
            case SCREEN_BOOT:
                showBootAnimation();
                currentScreen = SCREEN_GREETING;
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
            showDashboard(
                motion,
                light,
                elapsedSeconds
            );
            break;

            case SCREEN_IDLE:
            case SCREEN_BREAK:
            case SCREEN_MENU:
            case SCREEN_AI:
                break;
        }
    }
