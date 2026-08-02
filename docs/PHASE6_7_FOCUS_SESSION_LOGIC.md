# Phase 6.7 — Focus Session Logic

## 1. Overview

Phase 6.7 adds a dedicated business-logic session state machine. It determines Focus or Idle from existing PIR data, controls the existing timer pause/resume APIs, updates the existing dashboard status field, and prints serial messages only when the state changes.

No AI recommendation, automatic break scheduling, menu change, startup change, OLED layout change, or hardware pin change was made.

## 2. State Machine Design

```text
                  motion detected
SESSION_IDLE ----------------------> SESSION_FOCUS
     ^                                    |
     |                                    | no motion for 60 seconds
     +------------------------------------+

SESSION_BREAK is declared and reserved. No automatic transition enters it.
```

`SessionState` is an eight-bit enum owned by `session.h` / `session.cpp`. `session.cpp` is the single owner of the current state and its transitions.

## 3. Files Modified

- `include/config.h`
  - Added `SESSION_IDLE_TIMEOUT` with a value of 60 seconds because no idle-timeout configuration previously existed.
- `src/session.h`
  - Added the dedicated `SessionState` enum and session public API.
- `src/session.cpp`
  - Added PIR-driven state determination, transition filtering, timer control, and transition-only serial logging.
- `src/main.cpp`
  - Initializes the session module after the timer and supplies existing PIR motion/idle data once per loop.
- `src/ui.cpp`
  - Changes only the dashboard status value from a fixed `Studying...` string to the current session-state text.

## 4. Functions Modified

- `setup()` initializes the session layer after `initTimer()`.
- `loop()` calls `updateSession(motion, getIdleSeconds())` after the existing PIR and timer updates.
- `updateDashboardValues()` reads the authoritative session state only for dashboard presentation.

New session functions are `initSession()`, `updateSession()`, and `getSessionState()`.

## 5. Session Transition Rules

- Initial state: `SESSION_FOCUS`, preserving the existing running timer behavior.
- PIR motion detected: transition to `SESSION_FOCUS` if not already focused.
- No motion for `SESSION_IDLE_TIMEOUT` (60 seconds): transition to `SESSION_IDLE` if not already idle.
- `SESSION_BREAK` is reserved and is never entered automatically.
- Repeated input that requests the current state does nothing; no repeated timer calls or serial messages occur.

## 6. Timer Behavior

- `SESSION_FOCUS`: calls the existing `resumeSession()` API on transition from Idle.
- `SESSION_IDLE`: calls the existing `pauseSession()` API on transition from Focus.
- The timer is never reset by session logic.
- Returning to Focus adjusts the existing timer start point through `resumeSession()`, preserving elapsed study time.

## 7. Hardware Test Procedure

1. Build with `pio run` and upload to the Arduino Uno R3.
2. Open Serial Monitor at 9600 baud.
3. Verify the existing Boot, Greeting, Loading, Logo, and Main Menu sequence is unchanged.
4. Select Focus Session and confirm Dashboard status shows `Studying...`.
5. Trigger the PIR. Confirm Dashboard remains `Studying...` and no repeated focus messages are printed.
6. Leave the PIR inactive for at least 60 seconds after calibration. Confirm status changes to `Idle`, the timer stops, and Serial prints one `Session -> IDLE` line.
7. Trigger the PIR again. Confirm status changes to `Studying...`, the timer resumes without resetting, and Serial prints one `Session -> FOCUS` line.
8. Confirm UP, DOWN, SELECT, LDR percentage, and Dashboard layout remain unchanged.

## 8. Expected OLED Output

```text
Smart Study AI
---------------------
Motion : YES
Light  : 75%
Rec.   : Studying...
Time   : 00:42
```

When the inactivity timeout expires, only the status value changes:

```text
Rec.   : Idle
```

`SESSION_BREAK` is reserved for a later phase and would display `Rec.   : Break` if entered by future logic.

## 9. Expected Serial Monitor Output

```text
OLED initialized successfully.
Session -> IDLE
Session -> FOCUS
```

The session messages appear only on state transitions. No message is printed each loop iteration.

## 10. Memory Usage

PlatformIO Uno R3 build result:

- Flash: 17,746 / 32,256 B used; 14,510 B remaining.
- SRAM: 897 / 2,048 B used; 1,151 B remaining.
- Compile errors: 0.
- Compiler warnings: 0.
- Linker errors: 0.

The session state is stored as an 8-bit enum. No dynamic allocation or duplicate timer/PIR state was introduced.

## 11. Arduino Uno Q Compatibility

The session layer depends only on Arduino core APIs (`millis()`, `Serial`) and the existing module interfaces. It introduces no Uno R3-specific pin, register, or memory-layout dependency. For Uno Q migration, retain the 60-second configuration value and verify the existing PIR timing and serial behavior on the target board.
