# Changelog

## Release Candidate v1.0.0-rc1 — Arduino UNO Q + App Lab WebUI

### Added

**Core Functionality (Phase 8 Production Release)**
- Arduino UNO Q firmware using RouterBridge/RPClite communication
- Python App Lab bridge and device aggregation layer
- WebUI dashboard served via the `arduino:web_ui` Brick
- LLM-backed AI recommendation layer with deterministic fallback behavior
- Session control and idle-warning buzzer flow in the production architecture
- Production documentation and release readiness materials for Phase 8

**Release Packaging (Phase 9)**
- Comprehensive release notes (RELEASE.md)
- Developer quickstart guide (QUICKSTART.md)
- Phase 9.1 final release audit results (PHASE9_1_FINAL_RELEASE_AUDIT.md)
- Phase 9 release checklists and verification documentation
- Repository metadata (app.yaml, .gitignore, license files)

### Changed

**Architecture & Implementation**
- Reframed the project around the active UNO Q + WebUI architecture instead of the historical OLED prototype
- Clarified the role of the MCU recommendation layer as the fallback source of truth
- Kept AI as an enhancement layer rather than a point of failure
- Documented protocol versioning and API envelopes for telemetry, command, event, and error messages
- Hardened AI output handling to prevent overly verbose or malformed recommendation text from breaking dashboard rendering
- Updated README.md to accurately describe current architecture (UNO Q + WebUI, not OLED)

### Verified

**Automated Verification (Phase 9)**
- Python syntax validation passes (`python -m compileall`)
- AI hardening regression test passes under the repository environment
- Protocol envelope definitions verified for all message types
- Command whitelist validated against implementation
- Bridge communication interface verified complete

**Hardware Verification (Phase 8-9, completed by developer)**
- Arduino UNO Q firmware execution and sensor functionality
- PIR motion detection and idle state tracking
- LDR ambient light monitoring (0-100%)
- Buzzer operation and all cue patterns
- Idle warning buzzer with motion cancellation
- Session state machine (focus/idle/break)
- Bridge RPC communication verified functional
- WebUI dashboard responsive and interactive
- All five session control commands operational
- AI recommendation layer with fallback behavior
- Complete end-to-end integration validated

### Release Status

✅ **Ready for Release Candidate**

All technical requirements have been met:
- Core functionality complete and tested
- All sensors and actuators verified functional
- Communication reliable with proper fallback mechanisms
- AI integration safe and non-blocking
- WebUI responsive and user-friendly
- Security model documented for trusted-network deployment
- Documentation comprehensive and accurate
- Historical code and documentation properly archived

### Notes

**Documentation**
- Phase 9 includes comprehensive release documentation (RELEASE.md, QUICKSTART.md)
- Historical documentation for the OLED prototype remains in repository for development context
- All legacy documents marked with historical headers for clarity
- Current release focuses on UNO Q/WebUI implementation, not earlier SSD1306 display path

**Known Limitations**
- Current security model assumes trusted local or local-network deployment (no authentication)
- Requires Arduino App Lab environment to run
- LLM Brick availability determines AI feature availability; system gracefully falls back to rule-based recommendations

**Hardware Update**
- Earlier project phases used SSD1306 OLED display and push-button inputs
- Current production release has migrated to WebUI Brick for user interface
- Physical OLED and button hardware are no longer part of the active implementation

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
