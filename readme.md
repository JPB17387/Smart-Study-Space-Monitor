<h1 align="center">SMART STUDY SPACE MONITOR</h1>
<p align="center">Copyright: Jhon Paul Baonil 2026</p>
<br>
<p align="center">This is the main project repository of my project for Hackster Invent the Future Global Hackathon.</p>

**Sample Images of V1.0.0**<br>
<p align="center">
<img src="./docs/screenshots/img1.jpg" alt="Screenshot description" width="500">
<img src="./docs/screenshots/img2.jpg" alt="Screenshot description" width="500">
</p>

**Sample Image of V2.0.0**<br>
<p align="center">
<img src="./docs/screenshots/img3.png" alt="Screenshot description" width="500">
</p>

**Sample Image of V3.0.0**<br>
<p align="center">
<img src="./docs/screenshots/img4.png" alt="Screenshot description" width="500">
</p>

**Sample Video**<br>
<video width="640" height="360" align="center" controls>
  <source src="./docs/screenshots/vid1.mp4" type="video/mp4">
</video>

## Project Overview
Smart Study AI Platform is an Arduino Uno Q project that combines environmental sensing, session management, and a browser-based WebUI. The current release focuses on a live dashboard that displays motion, ambient light, session timing, and recommendation feedback through Arduino App Lab.

### Current Release Focus
- Arduino Uno Q WebUI dashboard
- Live telemetry from motion and light sensors
- Session control from the web interface
- Recommendation display and state feedback
- Bridge RPC communication between the MCU and the App Lab host

### Wiring:

#### PIR Sensor Wiring
```
PIR

VCC → 5V

GND → GND

OUT → D2
```

#### LDR Module Wiring
```
If your LDR module has pins:

AO
DO
GND
VCC

Connect:

VCC → 5V

GND → GND

AO → A0

Ignore DO for now.

We'll use the analog value.
```

#### Buzzer Wiring
```
GND → GND
VCC → D8
```

#### Main Project Wiring
```
UNO

5V
├── OLED VCC
├── PIR VCC
├── LDR VCC
└── BUZZER VCC

GND
├── OLED GND
├── PIR GND
|── LDR GND
└── BUZZER GND


D2 → PIR OUT

A0 → LDR AO
```

## Arduino Pin Assignments
```
| Component                  | Arduino Pin |
| -------------------------- | ----------- |
| PIR Sensor                 | D2          |
| Active Buzzer              | D8          |
| LDR Sensor (analog output) | A0          |
```

## Folder Structure

```
Smart Study Space Monitor
│
├── docs/
│   ├── 01_ARCHITECTURE_ANALYSIS.md
│   ├── 02_REFACTOR_PLAN.md
│   ├── 03_IMPLEMENTATION.md
│   ├── 04_TEST_REPORT.md
│   ├── PHASE6_6_IMPLEMENTATION.md
│   ├── PHASE6_7_FOCUS_SESSION_LOGIC.md
│   ├── PHASE6_OLED_ROOT_CAUSE_ANALYSIS.md
│   ├── architecture.md
│   ├── milestones.md
│   ├── roadmap.md
│   ├── screenshots/
│   └── public/
│
├── include/
│   └── config.h
│
├── lib/
│
├── src/
│   ├── main.cpp
│   ├── button.cpp
│   ├── button.h
│   ├── buzzer.cpp
│   ├── buzzer.h
│   ├── ldr.cpp
│   ├── ldr.h
│   ├── pir.cpp
│   ├── pir.h
│   ├── recommendation.cpp
│   ├── recommendation.h
│   ├── session.cpp
│   ├── session.h
│   ├── timer.cpp
│   ├── timer.h
│   ├── ui.cpp
│   └── ui.h
│
├── test/
│
├── platformio.ini
├── readme.md
├── CHANGELOG.md
├── LICENSE-APACHE
├── LICENSE-MIT
└── .gitignore
```

## Release Status

### v2.0.0 — WebUI Release
This release replaces the earlier OLED prototype experience with a browser-based dashboard for the Arduino Uno Q platform.

Implemented in this release:
- WebUI dashboard for live telemetry
- MCU-to-WebUI communication via Bridge RPC
- Motion and light sensor telemetry
- Session commands from the browser
- Recommendation output displayed in the dashboard

### Legacy Notes
The earlier v1.0.0 implementation used an SSD1306 OLED display and local button-based interaction. That path is now considered legacy and is no longer the primary release experience.

## Project Structure
The project now includes both embedded firmware and App Lab/WebUI components:
- `sketch/` for the Arduino Uno Q firmware
- `python/` for the host-side App Lab integration
- `assets/` for the browser dashboard
- `docs/` for architecture and implementation notes

## LICENSE 
[Apache License](LICENSE-APACHE)<br>
[MIT License](LICENSE-MIT)

## Notes

The current release is centered on the Uno Q WebUI experience rather than the legacy SSD1306 OLED interface.

## Release Documentation
- [Release Notes](RELEASE_NOTES.md)
- [Architecture Overview](docs/ARCHITECTURE_OVERVIEW.md)
- [Phase 6 Documentation](docs/PHASE6_DOCUMENTATION.md)
