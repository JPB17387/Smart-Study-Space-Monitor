# Phase 6 — OLED Root Cause Analysis

## 1. Root Cause

The OLED failure was AVR SRAM exhaustion during `Adafruit_SSD1306::begin()`. No individual initialization module caused the failure through a pin, Wire, constructor, or API conflict.

The SSD1306 library returns `false` immediately when its 128 x 64 framebuffer allocation fails:

```cpp
if ((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;
```

This allocation requires 1,024 bytes. Before the fix, the fully linked firmware already used 897 bytes of the Uno R3's 2,048-byte SRAM, leaving only 1,151 bytes before stack use. After the framebuffer allocation, only 127 bytes remained for the call stack and interrupts, causing the allocation to fail in the reported setup sequence.

## 2. Evidence

### Binary Isolation Result

The OLED-only test included an infinite loop after `initUI()`. Link-time optimization therefore removed the normal `loop()` dependency chain and its related module state. That test reported only 585 bytes of static SRAM.

The restored full firmware, with the normal loop and all module references reachable, reported:

- `.data`: 432 bytes
- `.bss`: 465 bytes
- Static SRAM: 897 bytes

This proves the comparison did not isolate an initialization function. It compared two different linked-memory images.

### Library Failure Path

The inspected Adafruit SSD1306 implementation performs framebuffer `malloc()` before `wire->begin()` and before display I2C initialization. Therefore a `false` return from `display.begin()` in this configuration indicates allocation failure, not an I2C address, Wire, pin, or peripheral initialization failure.

### Excluded Causes

- Duplicate display object: none; only one global `Adafruit_SSD1306 display` exists.
- Duplicate Wire initialization: none; no driver calls `Wire.begin()` or uses `Wire`.
- Pin conflict: none; OLED uses Uno R3 I2C pins A4/A5, while PIR uses D2, buttons D5/D6/D7, buzzer D8, and LDR A0.
- Interrupt conflict: none; no project ISR or interrupt attachment is present.
- Static initialization order: no driver constructor performs I2C access or dynamic allocation.
- Include, macro, linker, invalid-pointer, and duplicate-global issues: none caused the `display.begin()` failure path.

## 3. Files Inspected

- `platformio.ini`
- `include/config.h`
- Every header and source file under `src/`
- All global and static variables in project code
- Adafruit SSD1306 library `begin()` implementation
- AVR ELF `.data` and `.bss` sections and RAM symbols

## 4. Files Modified

- `src/main.cpp`
  - Restored the complete setup initialization sequence after the temporary OLED-only test.
- `src/ui.cpp`
  - Moved OLED and UI serial text from SRAM to program memory with `F()`.
  - Replaced the RAM-resident menu label array with an internal flash-resident menu-label helper.

## 5. Exact Code Changes

All OLED-visible strings retain their original text, order, and cursor positions. They now use Arduino's `F()` macro, which stores text in flash instead of copying it to `.data` SRAM.

The three Main Menu labels are printed through `showMenuItem()` using the existing `MENU_FOCUS`, `MENU_AI`, and `MENU_BREAK` order. This removes their RAM-resident pointer array and strings without changing navigation or rendering.

The restored `setup()` calls, in order:

```text
initPIR()
initLDR()
initButton()
initBuzzer()
initTimer()
initSession()
initUI()
```

## 6. Why the Bug Occurred

AVR string literals used by the original OLED rendering calls occupied the `.data` section. Together with normal runtime globals, they left inadequate free SRAM for the SSD1306's required dynamic framebuffer and execution stack.

The OLED-only experiment appeared to succeed because its infinite loop let the linker remove normal program references, substantially reducing static SRAM. The initialization calls themselves did not corrupt memory or interfere with I2C.

## 7. Why the Fix Works

The full firmware after the change reports:

- `.data`: 114 bytes
- `.bss`: 465 bytes
- Static SRAM: 579 bytes
- Flash: 17,822 / 32,256 bytes used

The 1,024-byte SSD1306 framebuffer now leaves approximately 445 bytes for stack and interrupt activity:

```text
2,048 - 579 - 1,024 = 445 bytes
```

This is 318 bytes more static-SRAM headroom than before. The fix changes storage location only; it does not alter OLED text, layout, I2C configuration, driver behavior, session logic, menu behavior, or public APIs.

## 8. Hardware Verification Checklist

- [ ] Upload the full firmware to the Arduino Uno R3.
- [ ] Confirm `OLED initialized successfully.` on Serial Monitor at 9600 baud.
- [ ] Confirm Boot Animation.
- [ ] Confirm Greeting screen.
- [ ] Confirm Loading screen.
- [ ] Confirm Logo screen.
- [ ] Confirm Main Menu and UP/DOWN/SELECT operation.
- [ ] Confirm Dashboard Motion, Light, status, and Time updates.
- [ ] Confirm PIR operation and Focus/Idle transition.
- [ ] Confirm LDR percentage changes.
- [ ] Confirm timer pause/resume behavior.
- [ ] Confirm buzzer operation.
- [ ] Confirm no unexpected serial output.

## 9. Remaining Risks

The SSD1306 framebuffer is still dynamically allocated, as required by the existing Adafruit SSD1306 library. The new 445-byte headroom is sufficient for the current firmware but must be monitored as features are added. All new display and serial strings should use `F()` or equivalent flash storage on the Uno R3.

Physical hardware verification remains required because this environment can compile and inspect ELF memory but cannot observe the connected OLED or Serial Monitor.

## 10. Arduino Uno Q Compatibility

The fix uses standard Arduino `F()` program-memory macros and does not depend on Uno R3 pin registers or modify hardware definitions. Uno Q has a different architecture and substantially different memory characteristics, but the code remains compatible. Retest the OLED and flash-string behavior after selecting the Uno Q PlatformIO environment.
