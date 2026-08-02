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
