# Architecture Analysis

## Summary

The firmware uses separate PIR, LDR, button, buzzer, timer, and UI modules. `main.cpp` initializes the modules and supplies their runtime data to the UI.

## Files Reviewed

- `include/config.h`
- All files under `src/`
- `platformio.ini`

## Current Data Flow

`main.cpp` updates PIR, LDR, timer, and button inputs, then calls `updateUI()` with motion, light percentage, elapsed seconds, and the latest `ButtonEvent`.

## Risks

- UI state and menu declarations had duplicate ownership before Milestone 1.
- Hardware validation is required after every build.

## Compatibility

Arduino Uno R3 pins, public APIs, startup order, and OLED layout are the baseline contracts.

## Build and Hardware Test Result

Milestone 1 build: successful. Hardware test: pending on the physical Uno R3.

## Memory Usage

Milestone 1: 17,052 / 32,256 B flash and 869 / 2,048 B SRAM.

## Rollback

Baseline source snapshot: `backup/Milestone1/`.

