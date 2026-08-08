# Changelog

## Unreleased — Milestone 1

### Fixed

- Removed duplicate UI state globals and conflicting menu-count ownership that prevented the firmware from compiling.

### Compatibility

- No hardware pins, public APIs, startup sequence, menu order, OLED layout, or display behavior changed.

### Verification

- `pio run` passed.
- Flash: 17,052 / 32,256 B used.
- SRAM: 869 / 2,048 B used.
- Physical Uno R3 verification remains pending.

### Rollback

- Pre-change snapshot: `backup/Milestone1/`.

## Phase 6 — Current Firmware Documentation

### Implemented
- OLED startup sequence and menu framework
- PIR motion sensing and idle state tracking
- LDR ambient light percentage conversion
- Debounced UP/DOWN/SELECT button navigation
- Buzzer feedback on button press
- Timer pause/resume behavior
- Flash-resident recommendation provider
- Dashboard rendering with static/dynamic refresh separation

### Notes
- `SCREEN_AI`, `SCREEN_BREAK`, and `SCREEN_IDLE` states are present but do not have dedicated screen render content in the current implementation.
- `SESSION_BREAK` is declared in `session.h` but is not entered by current PIR/session logic.
- `getLightStatus()` is implemented but not currently displayed.
