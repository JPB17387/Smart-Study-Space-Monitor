# Smart Study AI Platform — Testing Documentation

This document describes verification points that can be derived from the current code and build configuration.

## Build / Compile Test
- Status: NOT VERIFIED
- Notes: PlatformIO command is unavailable in the current terminal environment, so build execution could not be confirmed here.

## Startup Test
- Status: IMPLEMENTED
- Behavior: boot animation, greeting, loading screen, and logo screen are present in `ui.cpp`.
- Verification: code review confirms startup sequence.

## OLED Test
- Status: IMPLEMENTED
- Behavior: `initUI()` initializes the SSD1306 at `OLED_ADDRESS` and configures text settings.
- Verification: code review confirms `display.begin()` and startup rendering logic.

## Button Test
- Status: IMPLEMENTED
- Behavior: UP, DOWN, and SELECT buttons are debounced in `button.cpp` with `INPUT_PULLUP`.
- Notes: SELECT triggers a longer beep.
- Verification: button state machine is present in code.

## PIR Test
- Status: IMPLEMENTED
- Behavior: PIR sensor is initialized on `PIR_PIN` and calibrated for `PIR_CALIBRATION_TIME`.
- Verification: `updatePIR()` computes motion edges and idle seconds.

## LDR Test
- Status: IMPLEMENTED
- Behavior: LDR value is read from `LDR_PIN` and mapped to a 0–100% brightness scale.
- Verification: `getLightPercent()` and `getLightStatus()` are present.

## Buzzer Test
- Status: IMPLEMENTED
- Behavior: `initBuzzer()` sets `BUZZER_PIN` low and `beep()` pulses it for the requested duration.
- Verification: buzzer code is present and used by button handling.

## Timer Test
- Status: IMPLEMENTED
- Behavior: elapsed time is tracked with `millis()`, paused, and resumed without resetting during idle.
- Verification: `timer.cpp` provides `getElapsedSeconds()` and pause/resume semantics.

## Session-State Test
- Status: IMPLEMENTED
- Behavior: `SESSION_FOCUS` and `SESSION_IDLE` transitions are implemented based on PIR motion and idle timeout.
- Verification: code review confirms state transitions and timer control.

## Recommendation Test
- Status: IMPLEMENTED
- Behavior: recommendation strings are selected from flash memory using session state and light level.
- Verification: `recommendation.cpp` implements the rule set.

## Partial / Placeholder Behavior
- `SCREEN_AI`: NOT FULLY IMPLEMENTED
- `SCREEN_BREAK`: NOT FULLY IMPLEMENTED
- `SCREEN_IDLE`: NOT RENDERED
- `SESSION_BREAK`: RESERVED but not entered automatically

## Test Status Legend
- PASS: Verified from working hardware or build output.
- FAIL: Verified failure in current code.
- NOT VERIFIED: Not confirmed from the current workspace environment.
- IMPLEMENTED: present in source code and expected to work.
- NOT IMPLEMENTED: not present in the current implementation.

## Recommended Validation Steps
1. Build firmware with `pio run` in the project root.
2. Upload to Arduino Uno.
3. Open Serial Monitor at 9600 baud.
4. Verify OLED startup sequence completes and menu appears.
5. Use UP/DOWN/SELECT to change menu selection and open Focus Dashboard.
6. Observe PIR motion changes from `YES` to `NO` when the sensor is covered.
7. Confirm LDR percentage updates when ambient light changes.
8. Confirm SELECT returns from Dashboard to Main Menu.
9. Verify buzzer pulses on button press.
10. Confirm idle transition after 60 seconds of no motion.
