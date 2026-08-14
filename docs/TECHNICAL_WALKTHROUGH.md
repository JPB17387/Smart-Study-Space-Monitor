# HISTORICAL: Smart Study AI Platform Technical Walkthrough

⚠️ **HISTORICAL DOCUMENTATION**

This document describes the OLED-based implementation. The current production implementation uses the Arduino UNO Q + App Lab WebUI architecture. See [docs/ARCHITECTURE.md](ARCHITECTURE.md) for the current production architecture.

---

## Power On
1. The Arduino Uno receives power.
2. `setup()` executes.
3. Serial communication is started at 9600 baud.

## Hardware Initialization
1. `initPIR()` configures `PIR_PIN` as an input and begins PIR calibration.
2. `initLDR()` configures `LDR_PIN` as an analog input.
3. `initButton()` configures the three button pins with `INPUT_PULLUP`.
4. `initBuzzer()` configures `BUZZER_PIN` as an output and holds it LOW.
5. `initTimer()` starts the session timer from `millis()`.
6. `initSession()` initializes the session state to `SESSION_FOCUS`.
7. `initRecommendation()` initializes the recommendation provider.
8. `initUI()` initializes the SSD1306 OLED display.

## Startup Sequence
1. `runStartupSequence()` calls:
   - `showBootAnimation()`
   - `showGreeting()`
   - `showLoadingAnimation()`
   - `showLogo()`
2. After startup screens complete, `setScreen(SCREEN_MENU)` enters the main menu.

## Main Loop
Each `loop()` iteration performs:
1. `updatePIR()` to refresh PIR motion detection state.
2. `getLightPercent()` to read and map the LDR value.
3. `isMotionDetected()` to obtain the current motion boolean.
4. `updateTimer()` (reserved for future logic).
5. `updateSession(motion, getIdleSeconds())` to apply session state transitions.
6. `updateRecommendation(getSessionState(), light)` to select the current recommendation.
7. `updateButton()` to detect button presses and trigger buzzer feedback.
8. `updateUI(motion, light, getElapsedSeconds(), button)` to render the current screen.

## Main Menu
- `SCREEN_MENU` renders three items:
  - `Focus Session`
  - `AI Assistant`
  - `Break Mode`
- UP/DOWN buttons change `selectedMenu`.
- SELECT enters the chosen screen:
  - Focus Session → `SCREEN_DASHBOARD`
  - AI Assistant → `SCREEN_AI`
  - Break Mode → `SCREEN_BREAK`
- `SCREEN_AI` and `SCREEN_BREAK` currently do not draw dedicated content.

## Focus Session Screen
- `SCREEN_DASHBOARD` displays:
  - Motion status (`YES` / `NO`)
  - Light percentage
  - Recommendation text
  - elapsed study time in `MM:SS`
- The dashboard is rendered with static labels once and dynamic values refreshed every `DISPLAY_REFRESH_INTERVAL` milliseconds.
- SELECT returns to the main menu.

## Session State
- `SESSION_FOCUS` is the default on startup.
- Motion detected by PIR transitions the state to `SESSION_FOCUS`.
- No motion for `SESSION_IDLE_TIMEOUT` seconds transitions the state to `SESSION_IDLE`.
- On focus/idle transitions, `resumeSession()` or `pauseSession()` are called.
- `SESSION_BREAK` is defined but not entered automatically.

## Sensor Data
- PIR motion is read from digital pin `D2`.
- LDR light level is read from analog pin `A0`.
- Light is converted from 0–1023 to 100–0 percent.
- `getLightStatus()` returns a descriptive string but is not used in the current UI.

## Recommendation Provider
- The provider uses session state and ambient light to pick one of five flash-resident strings.
- Focus recommendations vary by light thresholds:
  - `>= 60%` → good focus
  - `>= 30%` → acceptable lighting
  - `< 30%` → raise lighting
- Idle and break states return separate static recommendations.

## UI Update Logic
- The display is owned by `ui.cpp`.
- `updateUI()` performs behavior based on `currentScreen`.
- Startup screens are advanced sequentially when the current screen is one of the startup states.
- Menu selection and dashboard rendering occur only in their active screen states.
- `SCREEN_AI`, `SCREEN_BREAK`, and `SCREEN_IDLE` are present but not rendered fully.

## Output Devices
- OLED: I2C display at address `0x3C`.
- Buzzer: digital output on pin `D8`.
- Serial: debug output on 9600 baud.

## State Machine Diagram
```mermaid
flowchart TD
    Start[Power On]
    Setup[setup()]
    PIR[PIR Init]
    LDR[LDR Init]
    Btn[Button Init]
    Buzzer[Buzzer Init]
    Timer[Timer Init]
    Session[Session Init]
    Rec[Recommendation Init]
    UI[UI Init]
    Startup[Startup Sequence]
    Menu[Main Menu]
    Dashboard[Dashboard]
    AI[AI Screen]
    Break[Break Screen]
    Idle[Idle Screen]

    Start --> Setup --> PIR --> LDR --> Btn --> Buzzer --> Timer --> Session --> Rec --> UI --> Startup --> Menu
    Menu -->|SELECT Focus| Dashboard
    Menu -->|SELECT AI| AI
    Menu -->|SELECT Break| Break
    Dashboard -->|SELECT| Menu
    AI -->|SELECT| Menu
    Break -->|SELECT| Menu
``` 

## Implementation Notes
- The OLED startup path is executed before the runtime menu.
- `updateUI()` currently handles screen transitions and redrawing within the same function.
- The timer uses `millis()` for elapsed time and supports pause/resume but not countdown timing.
- Button press beeps are immediate and blocking due to `delay()` in `beep()`.

## Current Gaps
- `SCREEN_AI`, `SCREEN_BREAK`, and `SCREEN_IDLE` are placeholders without dedicated drawing routines.
- `getLightStatus()` is available but not displayed.
- `updateTimer()` is reserved for future timer logic and does not affect runtime behavior.

## Source Protection
This document is based only on the current implementation and does not assume any code changes beyond what is present in the current workspace.
