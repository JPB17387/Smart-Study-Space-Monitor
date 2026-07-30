#include "ui.h"
#include "config.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//==================================================
// OLED CONFIGURATION
//==================================================

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

//==================================================
// UI STATE
//==================================================

static Screen currentScreen = SCREEN_BOOT;

SessionMode currentSession = SESSION_FOCUS;

static unsigned long lastMotionTime = 0;
static unsigned long sessionStartTime = 0;

static const unsigned long IDLE_TIMEOUT = 10000;

//==================================================
// INITIALIZATION
//==================================================

void initUI()
{
    Wire.begin();

    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed.");

        while (1)
        {
        }
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);
    display.setTextSize(1);
    display.display();

    currentScreen = SCREEN_BOOT;

    lastMotionTime = millis();
    sessionStartTime = millis();
}

//==================================================
// SCREEN CONTROL
//==================================================

void setScreen(Screen screen)
{
    currentScreen = screen;
}

//==================================================
// BOOT ANIMATION
//==================================================

void showBootAnimation()
{
    const int frames = 12;

    for (int frame = 0; frame < frames; frame++)
    {
        display.clearDisplay();

        display.setTextSize(1);

        display.setCursor(18, 6);
        display.println("SMART STUDY AI");

        display.setCursor(36, 22);
        display.println("Booting");

        display.drawRect(
            14,
            40,
            100,
            10,
            SSD1306_WHITE
        );

        int width = map(
            frame,
            0,
            frames - 1,
            0,
            98
        );

        display.fillRect(
            15,
            41,
            width,
            8,
            SSD1306_WHITE
        );

        display.setCursor(18, 56);
        display.print("Initializing...");

        display.display();

        delay(250);
    }

    currentScreen = SCREEN_GREETING;
}

//==================================================
// GREETING
//==================================================

void showGreeting()
{
    display.clearDisplay();

    display.setTextSize(2);

    display.setCursor(44, 16);
    display.println("Hi!");

    display.setTextSize(1);

    display.setCursor(20, 42);
    display.println("Welcome Sir Paul");

    display.display();

    delay(1800);

    currentScreen = SCREEN_LOADING;
}

//==================================================
// LOADING SCREEN
//==================================================

void showLoadingAnimation()
{
    for (int dots = 0; dots <= 6; dots++)
    {
        display.clearDisplay();

        display.setTextSize(1);

        display.setCursor(10, 12);
        display.println("Preparing Workspace");

        display.setCursor(35, 36);
        display.print("Loading");

        for (int i = 0; i < dots; i++)
        {
            display.print(".");
        }

        display.display();

        delay(220);
    }

    currentScreen = SCREEN_LOGO;
}

//==================================================
// LOGO
//==================================================

void showLogo()
{
    display.clearDisplay();

    display.drawRect(
        0,
        0,
        128,
        64,
        SSD1306_WHITE
    );

    display.setTextSize(1);

    display.setCursor(18, 22);
    display.println("SMART STUDY AI");

    display.setCursor(32, 38);
    display.println("Assistant");

    display.display();

    delay(2500);

    sessionStartTime = millis();
    lastMotionTime = millis();

    currentScreen = SCREEN_DASHBOARD;
}

//==================================================
// DASHBOARD
//==================================================

void showDashboard(
    bool motion,
    int light,
    SessionMode mode)
{
    display.clearDisplay();

    display.setTextSize(1);

    // Header
    display.setCursor(12, 0);
    display.println("SMART STUDY AI");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    // Motion
    display.setCursor(0, 18);
    display.print("Motion : ");
    display.println(
        motion ? "YES" : "NO"
    );

    // Light
    display.setCursor(0, 30);
    display.print("Light  : ");
    display.print(light);
    display.println("%");

    // Mode
    display.setCursor(0, 42);
    display.print("Mode   : ");

    switch (mode)
    {
        case SESSION_FOCUS:
            display.println("FOCUS");
            break;

        case SESSION_BREAK:
            display.println("BREAK");
            break;

        case SESSION_AI:
            display.println("AI");
            break;
    }

    display.setCursor(0, 56);
    display.println("Short Press = Menu");

    display.display();
}

//==================================================
// MENU
//==================================================

void showMenu(
    SessionMode mode)
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(34, 0);
    display.println("MAIN MENU");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    display.setCursor(8, 20);

    if (mode == SESSION_FOCUS)
        display.print("> ");
    else
        display.print("  ");

    display.println("Focus Session");

    display.setCursor(8, 34);

    if (mode == SESSION_BREAK)
        display.print("> ");
    else
        display.print("  ");

    display.println("Break Session");

    display.setCursor(8, 48);

    if (mode == SESSION_AI)
        display.print("> ");
    else
        display.print("  ");

    display.println("AI Assistant");

    display.display();
}

//==================================================
// FOCUS SCREEN
//==================================================

void showFocusScreen(
    unsigned long elapsedSeconds,
    bool motion,
    int light)
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(26, 0);
    display.println("FOCUS MODE");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    display.setCursor(0, 18);
    display.print("Time   : ");
    display.print(elapsedSeconds);
    display.println(" s");

    display.setCursor(0, 30);
    display.print("Motion : ");
    display.println(
        motion ? "YES" : "NO"
    );

    display.setCursor(0, 42);
    display.print("Light  : ");
    display.print(light);
    display.println("%");

    display.setCursor(0, 56);
    display.println("Short Press = Exit");

    display.display();
}

//==================================================
// BREAK SCREEN
//==================================================

void showBreakScreen(
    unsigned long elapsedSeconds,
    bool motion,
    int light)
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(30, 0);
    display.println("BREAK MODE");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    display.setCursor(0, 18);
    display.print("Break  : ");
    display.print(elapsedSeconds);
    display.println(" s");

    display.setCursor(0, 30);
    display.print("Motion : ");
    display.println(
        motion ? "YES" : "NO"
    );

    display.setCursor(0, 42);
    display.print("Light  : ");
    display.print(light);
    display.println("%");

    display.setCursor(0, 56);
    display.println("Relax & Hydrate");

    display.display();
}

//==================================================
// AI RECOMMENDATION
//==================================================

void showAIRecommendation(
    bool motion,
    int light)
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(8, 0);
    display.println("AI ASSISTANT");

    display.drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );

    display.setCursor(0, 18);

    if (light < 30)
    {
        display.println("Low lighting");
        display.println("Turn on desk lamp");
    }
    else if (!motion)
    {
        display.println("No movement");
        display.println("Take a short break");
    }
    else
    {
        display.println("Environment OK");
        display.println("Keep studying");
    }

    display.display();
}

//==================================================
// IDLE
//==================================================

void showIdleScreen()
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(34, 6);
    display.println("IDLE");

    display.drawLine(
        0,
        18,
        127,
        18,
        SSD1306_WHITE
    );

    display.setCursor(18, 30);
    display.println("No Motion");

    display.setCursor(8, 46);
    display.println("Waiting...");

    display.display();
}

//==================================================
// MAIN UI STATE MACHINE
//==================================================

void updateUI(
    bool motion,
    int light,
    ButtonEvent buttonEvent)
{
    //--------------------------------------------------
    // Motion tracking
    //--------------------------------------------------

    if (motion)
    {
        lastMotionTime = millis();
    }

    //--------------------------------------------------
    // Screen State Machine
    //--------------------------------------------------

    switch (currentScreen)
    {
        //--------------------------------------------------
        // Boot
        //--------------------------------------------------

        case SCREEN_BOOT:
            showBootAnimation();
            break;

        //--------------------------------------------------
        // Greeting
        //--------------------------------------------------

        case SCREEN_GREETING:
            showGreeting();
            break;

        //--------------------------------------------------
        // Loading
        //--------------------------------------------------

        case SCREEN_LOADING:
            showLoadingAnimation();
            break;

        //--------------------------------------------------
        // Logo
        //--------------------------------------------------

        case SCREEN_LOGO:
            showLogo();
            break;

        //--------------------------------------------------
        // Dashboard
        //--------------------------------------------------

        case SCREEN_DASHBOARD:

            showDashboard(
                motion,
                light,
                currentSession
            );

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_MENU;
            }

            if (!motion &&
                (millis() - lastMotionTime >= IDLE_TIMEOUT))
            {
                currentScreen = SCREEN_IDLE;
            }

            break;

        //--------------------------------------------------
        // Main Menu
        //--------------------------------------------------

        case SCREEN_MENU:

            showMenu(currentSession);

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                switch (currentSession)
                {
                    case SESSION_FOCUS:
                        currentSession = SESSION_BREAK;
                        break;

                    case SESSION_BREAK:
                        currentSession = SESSION_AI;
                        break;

                    case SESSION_AI:
                        currentSession = SESSION_FOCUS;
                        break;
                }
            }

            if (buttonEvent == BUTTON_LONG_PRESS)
            {
                sessionStartTime = millis();

                switch (currentSession)
                {
                    case SESSION_FOCUS:
                        currentScreen = SCREEN_FOCUS;
                        break;

                    case SESSION_BREAK:
                        currentScreen = SCREEN_BREAK;
                        break;

                    case SESSION_AI:
                        currentScreen = SCREEN_AI_RECOMMENDATION;
                        break;
                }
            }

            break;

        //--------------------------------------------------
        // Focus Session
        //--------------------------------------------------

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

        //--------------------------------------------------
        // Break Session
        //--------------------------------------------------

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

        //--------------------------------------------------
        // AI Recommendation
        //--------------------------------------------------

        case SCREEN_AI_RECOMMENDATION:

            showAIRecommendation(
                motion,
                light
            );

            if (buttonEvent == BUTTON_SHORT_PRESS)
            {
                currentScreen = SCREEN_DASHBOARD;
            }

            break;

        //--------------------------------------------------
        // Idle Mode
        //--------------------------------------------------

        case SCREEN_IDLE:

            showIdleScreen();

            if (motion)
            {
                lastMotionTime = millis();
                currentScreen = SCREEN_DASHBOARD;
            }

            break;

        //--------------------------------------------------
        // Safety
        //--------------------------------------------------

        default:
            currentScreen = SCREEN_DASHBOARD;
            break;
    }
}