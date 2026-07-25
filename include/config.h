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

#define PIR_PIN        2

// LDR (Phase 4)
#define LDR_PIN        A0

// Buzzer (Phase 5)
#define BUZZER_PIN     8

// Button (Phase 5)
#define BUTTON_PIN     7

//==================================================
// Timing
//==================================================

#define DISPLAY_REFRESH_INTERVAL 200
#define PIR_CALIBRATION_TIME     30000

#endif