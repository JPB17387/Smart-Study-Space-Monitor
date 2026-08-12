# Changelog

## Current Release — Arduino UNO Q + App Lab WebUI

### Added
- Arduino UNO Q firmware using RouterBridge/RPClite communication
- Python App Lab bridge and device aggregation layer
- WebUI dashboard served via the `arduino:web_ui` Brick
- LLM-backed AI recommendation layer with deterministic fallback behavior
- Session control and idle-warning buzzer flow in the production architecture
- Production documentation and release readiness materials for Phase 8

### Changed
- Reframed the project around the active UNO Q + WebUI architecture instead of the historical OLED prototype
- Clarified the role of the MCU recommendation layer as the fallback source of truth
- Kept AI as an enhancement layer rather than a point of failure
- Documented protocol versioning and API envelopes for telemetry, command, event, and error messages
- Hardened AI output handling to prevent overly verbose or malformed recommendation text from breaking dashboard rendering

### Verified locally
- Python AI hardening regression test passes under the repository environment
- protocol, App Lab, and AI-layer Python modules remain importable and structurally consistent
- documentation reflects the current architecture

### Historical note
- Older OLED-based project documents remain in the repository as historical references and are not the current production implementation.

## Phase 6 — Historical firmware documentation

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
- Historical documentation for the OLED prototype remains useful as context but does not describe the active release architecture.
- The current release focuses on the UNO Q/WebUI implementation, not the earlier SSD1306 display path.
