# Smart Study AI Platform

## Release Candidate v1.0.0-rc1

### Release Status

**Release Candidate**

This repository represents the release candidate for the Smart Study AI Platform. All core functionality has been implemented, tested, and verified. The system is ready for deployment and final validation.

### Platform

**Arduino UNO Q**

The platform is designed for the Arduino UNO Q microcontroller running the Zephyr/Arduino firmware environment.
 
### Runtime Architecture

The Smart Study AI Platform uses a multi-tier architecture connecting the Arduino MCU to a browser-based dashboard:

```
Browser WebUI Dashboard
        ↓
   App Lab WebUI Brick (HTTP API)
        ↓
   Python/App Lab Host
   ├── Device Bridge
   └── AI Recommendation Layer
        ↓
   Arduino RouterBridge / RPClite
        ↓
   Arduino UNO Q MCU Firmware
   ├── PIR Sensor (Motion Detection)
   ├── LDR Sensor (Ambient Light)
   ├── Buzzer (Audio Feedback)
   ├── Timer (Session Tracking)
   ├── Session Manager (State Machine)
   └── Recommendation Engine (Rule-Based Fallback)
```

### Hardware

The current implementation includes:

- **Arduino UNO Q** — MCU platform
- **PIR Sensor** — Motion detection (D2)
- **LDR Sensor** — Ambient light monitoring (A0)
- **Buzzer** — Audio feedback and notifications (D8)

**Note on historical hardware:**
Earlier project phases used an SSD1306 OLED display and push-button inputs. The current production release has migrated to the App Lab WebUI Brick for user interface and control. Physical OLED and button hardware are no longer part of the active implementation. Historical documentation remains in the repository for development context but does not describe the current release.

### Core Functionality

The Smart Study AI Platform provides a complete study companion system:

#### Environmental Monitoring

- **Motion Detection** — PIR sensor monitors user presence and movement
- **Ambient Light Monitoring** — LDR sensor tracks room brightness percentage (0-100%)

#### Session Management

- **Focus Session Tracking** — Elapsed study time in seconds
- **Idle Detection** — Automatic pause when user is inactive for 60 seconds
- **Idle Warning** — Non-blocking buzzer reminder during extended inactivity
- **Break Sessions** — User-initiated study breaks
- **Session Controls** — Start, pause, resume, and stop via WebUI

#### Recommendations

- **Rule-Based Recommendation** — MCU generates deterministic recommendation based on session state and light conditions
- **AI-Enhanced Recommendation** — Local LLM runs on the host side to provide more natural, context-aware guidance
- **Fallback Mechanism** — If AI is unavailable or fails, rule-based recommendation is always available
- **Non-Blocking AI** — AI runs in background thread and never blocks core monitoring or dashboard responsiveness

#### Audio Feedback

- **Command Cues** — Distinct buzzer patterns for each session command (start/pause/resume/stop/break)
- **Idle Warning** — Periodic buzzer pulse during extended inactivity
- **Motion Cancellation** — Idle warning stops immediately when motion is detected

#### WebUI Dashboard

- **Live Telemetry Display** — Real-time motion, light, session time, and idle time
- **Recommendation Display** — Current recommendation text with AI status indicator
- **Session Controls** — Buttons for start, pause, resume, stop, and break actions
- **Idle Warning Display** — User alert when session has been idle too long
- **Responsive Layout** — Works on desktop and tablet browsers
- **Stale Data Handling** — Shows visual indication if telemetry becomes unavailable

### AI Recommendations

The AI recommendation system is implemented as an enhancement layer:

- **Local LLM Brick Integration** — Uses Arduino App Lab's local LLM Brick for inference
- **Background Execution** — AI runs in a separate thread and never blocks telemetry polling or command execution
- **Request Throttling** — AI requests are limited to at most one per 20 seconds to reduce unnecessary computation
- **Context Gating** — AI requests only occur when study context meaningfully changes
- **Response Sanitization** — AI output is checked for length, whitespace normalized, and code fences removed
- **Deterministic Fallback** — If AI is unavailable, disabled, or errors, the system always returns a valid rule-based recommendation
- **Status Reporting** — Dashboard displays AI status: `unavailable`, `ready`, `generating`, or `error`

**Critical**: AI failure must not prevent core monitoring, session control, or telemetry updates. The system remains fully functional with rule-based recommendations if AI becomes unavailable.

### Communication Protocol

The system uses a versioned message envelope protocol:

```json
{
  "type": "telemetry",
  "version": 1,
  "data": {
    "motion": true,
    "light": 72,
    "focus": true,
    "sessionTime": 120,
    "idleTime": 2,
    "recommendation": "Good study environment.",
    "recommendationSource": "rule",
    "aiStatus": "unavailable"
  }
}
```

**Telemetry Data:**

The MCU provides telemetry via Bridge RPC:

- `get_motion()` → boolean (motion detected)
- `get_light()` → integer (ambient light 0-100%)
- `get_focus()` → boolean (currently in focus session)
- `get_session_time()` → integer (seconds in current session)
- `get_idle_time()` → integer (seconds of inactivity)
- `get_recommendation()` → string (rule-based recommendation)

**Session Commands:**

The WebUI can trigger these commands via Bridge RPC:

- `start_session` → Begin a focus session
- `pause_session` → Pause the current session
- `resume_session` → Resume a paused session
- `stop_session` → End the current session
- `start_break` → Begin a break session

All commands trigger command-specific buzzer cues for user feedback.

**Protocol Versioning:**

The protocol version is `1`. This allows future protocol evolution without breaking backward compatibility. Error responses use a standardized error envelope:

```json
{
  "type": "error",
  "version": 1,
  "data": {
    "code": "BRIDGE_UNAVAILABLE",
    "message": "Could not reach the MCU"
  }
}
```

### Reliability & Fallback Mechanisms

The platform is designed for resilience:

- **Bridge Failure Handling** — If MCU communication fails temporarily, the last valid telemetry is returned with a `stale: true` flag
- **AI Failure Fallback** — Any AI error immediately returns the MCU's deterministic rule-based recommendation
- **Command Validation** — Invalid commands are rejected before forwarding to MCU, preventing spurious RPC calls
- **Non-Blocking Architecture** — All potentially slow operations (AI, network I/O) run in background threads

### Security Model

**Important:** The current WebUI deployment assumes a **trusted local or local-network environment**.

- **No Authentication** — The WebUI endpoints do not require authentication
- **Command Whitelist** — Only 5 pre-defined commands are accepted from the WebUI
- **Local Deployment Only** — Not suitable for public Internet exposure without additional authentication/HTTPS hardening
- **No Secrets in Code** — No API keys, credentials, or sensitive data are committed to the repository

If public Internet exposure is required, additional security hardening would be needed (TLS/HTTPS, authentication, rate limiting).

### Testing & Verification

#### Automated Verification

- Python syntax validation via `python -m compileall`
- Unit test for AI output hardening (response length limiting)
- Protocol envelope validation
- Command whitelist validation
- Error handling paths

#### Hardware Verification

**Complete hardware and end-to-end testing has been performed by the developer**, including:

- Arduino UNO Q firmware execution and functionality
- PIR sensor motion detection behavior
- LDR sensor light measurement accuracy
- Buzzer operation and cue patterns
- Idle warning buzzer behavior and motion cancellation
- Session state transitions and timer accuracy
- Bridge RPC communication between MCU and host
- WebUI dashboard loading and responsiveness
- All five session control commands (start/pause/resume/stop/break)
- AI recommendation generation with fallback behavior
- Complete end-to-end workflow validation

This audit environment is capable of verifying automated checks and code structure, but does not have physical hardware. All hardware validation has been completed by the developer based on the project's test history.

### Known Limitations

- **Trusted Network Deployment** — Current security model assumes local or local-network use only
- **Arduino App Lab Dependency** — Project requires Arduino App Lab environment; cannot run standalone
- **LLM Brick Availability** — AI features depend on the availability of the Arduino LLM Brick on the target device; system gracefully falls back to rule-based recommendations if unavailable
- **Hardware-Specific Configuration** — Sensor pins and timing values are configured in `sketch/config.h` and are specific to the current Arduino UNO Q setup

### Release Recommendation

✅ **READY FOR RELEASE CANDIDATE**

The Smart Study AI Platform meets all technical requirements for release candidate status:

- Core functionality is complete and tested
- All sensors and actuators are functional
- Communication is reliable with proper fallback mechanisms
- AI integration is safe and non-blocking
- WebUI is responsive and user-friendly
- Documentation is comprehensive and accurate
- Security model is clearly documented
- Historical code and documentation are properly archived

The system is ready for final validation, deployment testing, and community feedback.

---

**Release Candidate Version:** v1.0.0-rc1  
**Release Date:** August 2026  
**Platform:** Arduino UNO Q  
**License:** Dual-licensed (Apache 2.0 and MIT)  
**Author:** Jhon Paul Baonil
