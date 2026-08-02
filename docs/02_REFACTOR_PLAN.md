# Refactor Plan

## Summary

Stabilize the existing firmware incrementally without changing pins, public APIs, startup order, menu order, OLED layout, or hardware behavior.

## Milestones

1. Remove duplicate UI globals and conflicting constants.
2. Remove obsolete UI state and dead code while preserving screen behavior.
3. Use `config.h` as the OLED configuration source of truth.
4. Improve driver documentation, formatting, and readability only.
5. Run full Uno R3 integration testing.

## Compatibility and Risks

Each milestone must compile before the next starts. UI changes are the highest risk because they affect visible navigation and display behavior.

## Testing and Memory Tracking

Every successful build records flash and SRAM use. OLED, buttons, PIR, LDR, timer, buzzer, and serial output require physical Uno R3 verification.

## Rollback

Create `backup/MilestoneN/` before each implementation milestone. Restore only the affected milestone snapshot if a regression occurs.

