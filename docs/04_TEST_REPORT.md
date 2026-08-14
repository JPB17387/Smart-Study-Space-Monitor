# HISTORICAL: Test Report

⚠️ **HISTORICAL DOCUMENTATION**

This document describes testing from an earlier phase. The current production implementation uses the Arduino UNO Q + App Lab WebUI architecture and has already completed end-to-end hardware and integration validation. See [docs/ARCHITECTURE.md](ARCHITECTURE.md) for the current production architecture.

---


## Milestone 1

### Build Test

- Command: `pio run`
- Result: passed
- Compile errors: 0
- Linker errors: 0
- Reported compiler warnings: 0

### Memory Usage

- Flash: 17,052 B used, 15,204 B remaining.
- SRAM: 869 B used, 1,179 B remaining.

### Physical Uno R3 Checklist

- [ ] OLED initialization
- [ ] Boot animation
- [ ] Greeting
- [ ] Loading animation
- [ ] Logo
- [ ] Main menu
- [ ] Dashboard
- [ ] UP button
- [ ] DOWN button
- [ ] SELECT button
- [ ] PIR
- [ ] LDR
- [ ] Timer running, pause, resume, and elapsed time
- [ ] Buzzer button feedback and beep
- [ ] Serial initialization/debug output

### Compatibility and Remaining Risk

Firmware behavior cannot be fully accepted until this checklist is run on the actual Uno R3. No hardware compatibility change was made.

### Rollback

Use `backup/Milestone1/` to restore the pre-refactor source snapshot.

