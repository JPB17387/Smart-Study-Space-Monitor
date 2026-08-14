# Smart Study AI Platform — Quickstart Guide

## What Is This Project?

The Smart Study AI Platform is a study companion device that combines environmental sensing, session management, and AI-assisted recommendations to help students focus better.

The system monitors:
- **Your presence** (motion sensor)
- **Your lighting** (ambient light sensor)
- **Your study time** (timer)

And provides:
- **Real-time feedback** via WebUI dashboard
- **Audio notifications** via buzzer
- **AI-enhanced recommendations** for improving your study environment
- **Session management** (start/pause/resume/stop focus sessions)

## Required Hardware

### Hardware Components

- **Arduino UNO Q** microcontroller
- **PIR motion sensor** (connected to D2)
- **LDR ambient light sensor** (connected to A0)
- **Buzzer** (connected to D8)
- **USB cable** for power and programming
- **5V power supply** for the breadboard/components
- **Breadboard and jumper wires** for connections

### Arduino Setup

The project targets the **Arduino UNO Q** with **Zephyr** Arduino core support. Ensure your development environment supports:

- Arduino App Lab
- Zephyr Arduino Core
- Python 3.7+

## Required Software

### On Your Development Machine

1. **Arduino CLI** or **Arduino IDE** with UNO Q support
2. **Arduino App Lab** environment (for running the Python host and WebUI)
3. **Python 3.7+** with the following available packages:
   - (dependencies are managed by Arduino App Lab)

### On the Device

- Arduino Zephyr-based firmware (compiled and uploaded via Arduino IDE/CLI)
- Arduino App Lab runtime
- Python host application

## Project Repository Structure

```
Smart Study Space Monitor/
├── sketch/                    # Arduino MCU firmware
│   ├── main.cpp              # Firmware entry point
│   ├── communication.cpp/.h   # Bridge RPC interface
│   ├── pir.cpp/.h            # PIR sensor driver
│   ├── ldr.cpp/.h            # LDR sensor driver
│   ├── buzzer.cpp/.h         # Buzzer control
│   ├── timer.cpp/.h          # Session timer
│   ├── session.cpp/.h        # Session state machine
│   ├── recommendation.cpp/.h # Rule-based recommendations
│   ├── config.h              # Configuration (pins, timing)
│   └── sketch.yaml           # Sketch profile for Zephyr
│
├── python/                    # App Lab Python host
│   ├── main.py               # App Lab entry point
│   ├── device.py             # Device bridge (MCU aggregator)
│   ├── protocol.py           # Message protocol definitions
│   └── ai_recommendation.py  # AI recommendation layer
│
├── assets/                    # WebUI files
│   └── index.html            # Dashboard HTML/CSS/JavaScript
│
├── app.yaml                   # App Lab configuration
├── docs/                      # Documentation
├── test/                      # Test files
├── readme.md                  # Project README
└── CHANGELOG.md              # Version history
```

## Building & Running

### Step 1: Prepare the Arduino UNO Q

1. Connect the hardware components:
   - PIR sensor OUT → D2
   - LDR sensor AO → A0
   - Buzzer + → D8
   - All components GND → GND
   - All components VCC → 5V

2. Connect the Arduino UNO Q to your development machine via USB

### Step 2: Build and Upload Firmware

Using Arduino IDE:

1. Open the `sketch/sketch.ino` file
2. Select **Board:** Arduino UNO Q
3. Select **Port:** (your USB port)
4. Click **Upload**

The firmware includes all drivers (PIR, LDR, buzzer) and will begin running automatically.

### Step 3: Run the Python/App Lab Host

In the Arduino App Lab environment:

```bash
# Navigate to the project directory
cd "Smart Study Space Monitor"

# Start the App Lab application
# (specific command depends on your Arduino App Lab setup)
```

The Python application will:
1. Start the WebUI Brick HTTP server
2. Initialize the Device Bridge
3. Connect to the MCU via RouterBridge/RPClite
4. Begin polling telemetry and serving the dashboard

### Step 4: Access the WebUI Dashboard

Once the App Lab application is running:

1. Open your browser
2. Navigate to the WebUI endpoint (typically `http://localhost:3000` or as configured by App Lab)
3. The live dashboard should load and display:
   - Motion status
   - Ambient light percentage
   - Current session time
   - Idle time
   - AI recommendation
   - Session control buttons

## Operating a Focus Session

### Starting a Focus Session

1. On the WebUI dashboard, click **Start Focus**
2. The buzzer emits a **start cue** (200ms pulse)
3. The timer begins counting elapsed seconds
4. Motion is monitored; if you move, the session remains in focus

### During a Focus Session

**If you're active (motion detected):**
- Timer continues to count
- Dashboard displays "Focus" status
- Recommendation updates based on light conditions

**If you become inactive (no motion for 60 seconds):**
- Timer is automatically paused
- Session enters Idle state
- Dashboard displays "Idle" status with idle time counter
- Buzzer emits a periodic reminder pulse every 2 seconds
- The recommendation may suggest returning to focus

### Pausing a Session

Click **Pause** to manually pause without waiting for idle timeout:
- Buzzer emits a **pause cue** (100ms pulse)
- Timer stops
- Session state remains in focus internally but timer is paused

### Resuming a Session

Click **Resume** to continue:
- Buzzer emits a **resume cue** (90ms + 90ms pulses)
- Timer resumes counting from where it paused

### Taking a Break

Click **Start Break**:
- Buzzer emits a **break cue** (150ms + gap + 150ms pulses)
- Session enters break state
- Timer stops
- No idle warnings during breaks

### Stopping a Session

Click **Stop**:
- Buzzer emits a **stop cue** (250ms pulse)
- Timer ends
- Session is reset to idle state

## How Sensors Work

### PIR Motion Sensor

- **Input:** Passive infrared motion detection
- **Output:** Binary (motion detected or no motion)
- **Usage:** Determines focus/idle state transitions
- **Behavior:**
  - On startup, calibrates for 30 seconds (ignores motion)
  - After calibration, detects any movement
  - Idle timer resets to 0 whenever motion is detected
  - If no motion for 60 seconds, session transitions to Idle

### LDR Ambient Light Sensor

- **Input:** Analog light level
- **Output:** Percentage (0-100%)
- **Usage:** Influences recommendations; may suggest adjusting lighting
- **Behavior:**
  - Continuously read and converted to brightness percentage
  - 0% = very dark; 100% = very bright
  - Updated on every dashboard poll
  - Used by AI to suggest optimal lighting for study

## How Buzzer Works

The buzzer provides non-blocking audio feedback. It does not use blocking delays in the communication path, ensuring responsiveness.

### Command Cues

- **Start Focus:** 200ms pulse
- **Pause:** 100ms pulse
- **Resume:** 90ms + gap + 90ms pulses
- **Stop:** 250ms pulse
- **Start Break:** 150ms + gap + 150ms pulses

### Idle Warning

During extended inactivity (60+ seconds idle):
- Buzzer emits a short 70ms pulse every 2 seconds
- Warning stops immediately when motion is detected
- Does not affect session state or telemetry

### Priority

Command cues have highest priority. If a command is issued, idle warnings pause until the command cue finishes.

## How AI Recommendations Work

### Rule-Based Baseline

The MCU (Arduino firmware) always generates a deterministic recommendation based on:
- Session state (focus/idle/break)
- Ambient light percentage

Example: "Keep the light at 70-80% for optimal focus" or "Return to your study after your break."

### AI Enhancement

On the host side, the Python layer can optionally use the Arduino LLM Brick to generate a more natural, context-aware recommendation:

1. **Context Gathering** — Reads current motion, light, session time, and idle time from MCU
2. **AI Request** — Sends a prompt to the LLM Brick with the context and rule-based recommendation
3. **Background Execution** — AI runs in a separate thread and does not block telemetry or dashboard updates
4. **Throttling** — AI requests are limited to at most one per 20 seconds to save resources
5. **Caching** — Results are cached until the study context meaningfully changes

**Example AI output:**
"Your study environment looks good—keep the focus and remember to take breaks every 45 minutes."

### AI Status Display

The dashboard shows AI status:
- **"AI ACTIVE"** — AI has generated a recommendation
- **"AI thinking…"** — AI is currently processing a request
- **"Using local study rules"** — AI is unavailable; using rule-based recommendation

### AI Fallback

If the LLM Brick is not available or disabled:
- The system automatically returns the MCU's rule-based recommendation
- No user action needed
- Dashboard continues to function normally
- AI status shows "unavailable"

**Critical:** AI is never required for the system to function. It is an enhancement layer that gracefully degrades.

## Testing the System

### Quick Verification

1. **Motion Detection**
   - Move in front of the PIR sensor
   - Dashboard "Motion" should toggle to "Detected"

2. **Light Monitoring**
   - Change room brightness (cover/uncover light sensor)
   - Dashboard "Light" percentage should change

3. **Buzzer Feedback**
   - Start a focus session
   - You should hear a 200ms pulse

4. **Session Timer**
   - Start a focus session
   - "Session time" counter should increment
   - After 60 seconds without motion, session should transition to Idle

5. **Idle Warning**
   - Wait in Idle state for a few seconds
   - You should hear periodic 70ms pulses from the buzzer

6. **Commands**
   - Click pause/resume/stop buttons
   - Verify corresponding buzzer cues play
   - Verify dashboard state updates

## Troubleshooting

### WebUI Dashboard Not Loading

- Verify the Python App Lab application is running
- Check that the Arduino UNO Q is powered and connected
- Verify the Browser can reach the App Lab WebUI endpoint
- Check Python logs for errors connecting to Bridge

### No Telemetry Updates

- Verify MCU is connected via USB
- Check that the Bridge communication is initialized
- Verify PIR sensor is receiving power (LED usually lights)
- Check that config.h has correct pin assignments (D2 for PIR, A0 for LDR)

### Buzzer Not Making Sound

- Verify buzzer positive wire is connected to D8
- Check that the buzzer ground is connected to GND
- Test buzzer with a simple digitalWrite test
- Verify buzzer polarity (+ to pin, - to ground)

### AI Recommendations Not Appearing

- Verify the LLM Brick is available in your App Lab configuration
- Check Python logs for LLM initialization errors
- If LLM is unavailable, system continues with rule-based recommendations (normal fallback)

## Project Documentation

For more detailed information, see:

- [RELEASE.md](RELEASE.md) — Release notes and architecture overview
- [docs/ARCHITECTURE.md](ARCHITECTURE.md) — Current production architecture
- [docs/AI_ARCHITECTURE.md](AI_ARCHITECTURE.md) — AI layer design and fallback behavior
- [docs/COMMUNICATION_PROTOCOL.md](COMMUNICATION_PROTOCOL.md) — Protocol specification and RPC interface
- [CHANGELOG.md](../CHANGELOG.md) — Version history
- [docs/PHASE9_1_FINAL_RELEASE_AUDIT.md](PHASE9_1_FINAL_RELEASE_AUDIT.md) — Release audit results

---

**For support or contributions, refer to the main [README](../readme.md).**

Happy studying! 📚🚀
