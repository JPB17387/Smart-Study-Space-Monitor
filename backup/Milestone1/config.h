#ifndef CONFIG_H
#define CONFIG_H

//==================================================
// Project Information
//==================================================

#define PROJECT_NAME "Smart Study AI Platform"

//==================================================
// OLED Display
//==================================================

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_ADDRESS   0x3C

//==================================================
// Pins
//==================================================

#define PIR_PIN            2

// LDR 
#define LDR_PIN            A0

// Buzzer 
#define BUZZER_PIN         8

// Button 
#define BUTTON_UP_PIN      6
#define BUTTON_SELECT_PIN  7
#define BUTTON_DOWN_PIN    5

//==================================================
// Timing
//==================================================

#define DISPLAY_REFRESH_INTERVAL 200
#define PIR_CALIBRATION_TIME     30000

#endif