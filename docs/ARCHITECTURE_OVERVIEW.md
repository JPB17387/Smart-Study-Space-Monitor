# HISTORICAL: Smart Study AI Platform — OLED Architecture

⚠️ **HISTORICAL DOCUMENTATION**

This document describes an earlier OLED-based implementation of the Smart Study AI Platform. The current production implementation uses the Arduino UNO Q + App Lab WebUI architecture and does not use this OLED-based design. See [docs/ARCHITECTURE.md](ARCHITECTURE.md) for the current production architecture.

---

## Project Scope
This document describes a legacy firmware architecture for the Smart Study AI Platform.

The legacy firmware ran on an Arduino Uno using the Arduino core and Adafruit SSD1306 OLED display library. It coordinated:
- PIR motion sensing
- LDR ambient light sensing
- push-button navigation
- buzzer notifications
- elapsed study timer
- on-screen menu/dashboard UI
- recommendation selection logic

## Module Summary

### `src/main.cpp`
- Purpose: application coordinator and runtime loop.
- Responsibilities:
  - initialize all hardware modules and software subsystems
  - run the startup UI sequence
  - update sensors, timer, session state, recommendation, button input, and UI each loop
- Public functions:
  - `setup()`
  - `loop()`
- Dependencies:
  - `ui.h`
  - `pir.h`
  - `ldr.h`
  - `timer.h`
  - `session.h`
  - `recommendation.h`
  - `button.h`
  - `buzzer.h`
- Inputs: sensor state, device time, button events
- Outputs: UI updates, serial diagnostics
- Interaction: central coordinator for all modules

### `src/ui.cpp` / `src/ui.h`
- Purpose: OLED display management and screen rendering.
- Responsibilities:
  - initialize the SSD1306 display
  - run startup screens: boot, greeting, loading, logo
  - render main menu and dashboard
  - manage screen state transitions
  - refresh dynamic dashboard values at a fixed interval
- Public functions:
  - `void initUI()`
  - `void runStartupSequence()`
  - `void updateUI(bool motion, int light, unsigned long elapsedSeconds, ButtonEvent button)`
  - `void setScreen(Screen screen)`
  - `Screen getCurrentScreen()`
- Important variables/state:
  - `currentScreen`
  - `selectedMenu`
  - `screenNeedsRedraw`
  - `menuNeedsRedraw`
  - `lastDashboardRefresh`
- Dependencies:
  - `config.h`
  - `recommendation.h`
  - `Adafruit_SSD1306` / `Adafruit_GFX`
- Inputs: motion, light level, elapsed study time, button events, recommendation text
- Outputs: OLED rendering, display state
- Interaction: consumes recommendation and session state indirectly via `getRecommendation()`

### `src/pir.cpp` / `src/pir.h`
- Purpose: PIR motion sensing and idle-time tracking.
- Responsibilities:
  - configure PIR input pin
  - calibrate PIR sensor on startup for `PIR_CALIBRATION_TIME`
  - detect motion edge transitions
  - count motion events
  - compute idle seconds since last motion
- Public functions:
  - `void initPIR()`
  - `void updatePIR()`
  - `bool isMotionDetected()`
  - `bool isPIRCalibrated()`
  - `unsigned long getMotionCount()`
  - `unsigned long getIdleSeconds()`
- Important variables/state:
  - `motion`
  - `previousMotion`
  - `calibrated`
  - `lastMotionTime`
  - `motionCounter`
- Inputs: digital read from `PIR_PIN`
- Outputs: motion state and idle duration
- Interaction: provides motion state to session manager and UI

### `src/ldr.cpp` / `src/ldr.h`
- Purpose: ambient light sensing.
- Responsibilities:
  - configure analog input pin for the LDR
  - read analog light value
  - convert raw sensor value to a brightness percentage
  - provide a readable light status string
- Public functions:
  - `void initLDR()`
  - `int readLDR()`
  - `int getLightPercent()`
  - `const char* getLightStatus()`
- Important variables/state: no persistent state besides the pin configuration
- Inputs: analog read from `LDR_PIN`
- Outputs: light percentage and status label
- Interaction: provides output to UI and recommendation engine

### `src/button.cpp` / `src/button.h`
- Purpose: push-button input scanning and debounced event detection.
- Responsibilities:
  - configure UP, DOWN, SELECT pins with `INPUT_PULLUP`
  - debounce each button using `DEBOUNCE_TIME`
  - emit `ButtonEvent` values on valid presses
  - trigger beep notifications on press
- Public functions:
  - `void initButton()`
  - `ButtonEvent updateButton()`
- Important variables/state:
  - `ButtonState upButton`
  - `ButtonState downButton`
  - `ButtonState selectButton`
- Inputs: digital reads from `BUTTON_UP_PIN`, `BUTTON_DOWN_PIN`, `BUTTON_SELECT_PIN`
- Outputs: `ButtonEvent` values and buzzer beeps
- Interaction: directly affects UI navigation and menu selection

### `src/buzzer.cpp` / `src/buzzer.h`
- Purpose: audible feedback generation.
- Responsibilities:
  - configure buzzer output pin
  - produce a fixed-duration beep
- Public functions:
  - `void initBuzzer()`
  - `void beep(unsigned int durationMs = 100)`
- Outputs: digital pulse on `BUZZER_PIN`
- Interaction: used from button handling only

### `src/timer.cpp` / `src/timer.h`
- Purpose: elapsed study time tracking.
- Responsibilities:
  - store session start time
  - track paused/resumed timer state
  - compute elapsed milliseconds and seconds
- Public functions:
  - `void initTimer()`
  - `void updateTimer()`
  - `void startSession()`
  - `void stopSession()`
  - `void pauseSession()`
  - `void resumeSession()`
  - `void resetSession()`
  - `bool isSessionRunning()`
  - `bool isSessionPaused()`
  - `unsigned long getElapsedMilliseconds()`
  - `unsigned long getElapsedSeconds()`
- Important variables/state:
  - `sessionStartTime`
  - `pausedTime`
  - `running`
  - `paused`
- Inputs: `millis()`, session state transitions
- Outputs: elapsed time and pause/resume control
- Interaction: used by session manager and UI

### `src/session.cpp` / `src/session.h`
- Purpose: PIR-driven session state management.
- Responsibilities:
  - maintain current session state as `SESSION_FOCUS`, `SESSION_IDLE`, or `SESSION_BREAK`
  - transition to `SESSION_FOCUS` on motion
  - transition to `SESSION_IDLE` after `SESSION_IDLE_TIMEOUT`
  - pause or resume timer when entering idle/focus
- Public functions:
  - `void initSession()`
  - `void updateSession(bool motion, unsigned long idleSeconds)`
  - `SessionState getSessionState()`
- Important variables/state:
  - `currentSessionState`
- Inputs: motion boolean and idle seconds from PIR driver
- Outputs: session state changes and timer control
- Interaction: supplies state to recommendation provider and UI

### `src/recommendation.cpp` / `src/recommendation.h`
- Purpose: rule-based recommendation text generation.
- Responsibilities:
  - initialize recommendation state
  - select recommendation text based on session state and ambient light
  - preserve flash-resident string pointers
  - expose change detection to the UI
- Public functions:
  - `void initRecommendation()`
  - `void updateRecommendation(SessionState state, int lightPercent)`
  - `const __FlashStringHelper* getRecommendation()`
  - `bool hasRecommendationChanged()`
- Important variables/state:
  - `currentRecommendation`
  - `recommendationChanged`
- Inputs: session state and light percentage
- Outputs: flash-string pointer and change flag
- Interaction: UI consumes recommendation text for display

## Main Application Flow

1. `setup()` initializes serial, sensors, buzzer, timer, session, recommendation, and UI.
2. `runStartupSequence()` renders boot, greeting, loading, and logo screens.
3. `loop()` performs:
   - `updatePIR()`
   - `getLightPercent()`
   - `isMotionDetected()`
   - `updateTimer()`
   - `updateSession(motion, getIdleSeconds())`
   - `updateRecommendation(getSessionState(), light)`
   - `updateButton()`
   - `updateUI(motion, light, getElapsedSeconds(), button)`

## UI Architecture

- `ui.cpp` owns the OLED display object and screen state.
- Screens are enumerated as:
  - `SCREEN_BOOT`
  - `SCREEN_GREETING`
  - `SCREEN_LOADING`
  - `SCREEN_LOGO`
  - `SCREEN_MENU`
  - `SCREEN_DASHBOARD`
  - `SCREEN_AI`
  - `SCREEN_BREAK`
  - `SCREEN_IDLE`
- The startup sequence advances through boot, greeting, loading, and logo once, then enters the menu.
- `SCREEN_MENU` supports UP/DOWN navigation and SELECT item activation.
- `SCREEN_DASHBOARD` refreshes only dynamic fields at `DISPLAY_REFRESH_INTERVAL`.
- `SCREEN_AI` and `SCREEN_BREAK` are reserved states with button return handling but no dedicated rendering in the current implementation.
- `SCREEN_IDLE` is declared but not rendered in the current code.

## Sensor and Driver Topology

- PIR and LDR modules are read before session and recommendation logic.
- Button input is read after recommendation selection and before UI update.
- Buzzer feedback is only triggered inside `updateButton()`.
- The timer module is paused and resumed from session state transitions only.

## Dependency Diagram

```mermaid
flowchart TD
    PIR[PIR Sensor]\n    LDR[LDR Sensor]\n    BUTTON[Buttons]\n    TIMER[Timer]\n    SESSION[Session Manager]\n    REC[Recommendation Provider]\n    UI[UI / OLED]\n    BUZZER[Buzzer]

    PIR --> SESSION
    LDR --> REC
    TIMER --> SESSION
    SESSION --> REC
    REC --> UI
    BUTTON --> UI
    BUTTON --> BUZZER
    UI --> OLED
    TIMER --> UI
``` 

## Module Dependency Notes

- `main.cpp` includes all driver and subsystem headers and orchestrates execution.
- `ui.cpp` includes `recommendation.h` to fetch current recommendation text.
- `session.cpp` includes `timer.h` and `config.h` for timer control and idle timeout.
- `pir.cpp`, `ldr.cpp`, and `button.cpp` include `config.h` for pin definitions.
- `recommendation.cpp` includes `session.h`.

## Memory and Optimization Summary

- Flash-resident text is used heavily in `ui.cpp` via `F()` macro.
- Recommendation strings are stored in `PROGMEM`.
- The SSD1306 display object is a single static global instance.
- No dynamic allocation is performed in application code.
- `timer.cpp` uses `millis()` for elapsed time and pause/resume logic.

## Findings

- `SCREEN_AI`, `SCREEN_BREAK`, and `SCREEN_IDLE` are declared but not fully rendered.
- `SESSION_BREAK` exists in the session enum but is not entered by current PIR/session logic.
- Recommendation text is produced in flash and exposed for direct UI drawing.
- The current architecture does not expose a separate display abstraction beyond `ui.cpp`.
