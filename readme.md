<h1 align="center">SMART STUDY SPACE MONITOR</h1>
<p align="center">Copyright: Jhon Paul Baonil 2026</p>
> This is the main project repository of my project for Hackster Invent the Future Global Hackathon.

## Important Details
Displays Address: 0x3C

### Wiring:

#### OLED Wiring
```
OLED

GND → GND

VCC → 5V

SCL → A5

SDA → A4
```

#### PIR Sensor Wiring
```
PIR

VCC → 5V

GND → GND

OUT → D2
```

#### LDR Module 
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

#### Main Project Wiring
```
UNO

5V
├── OLED VCC
├── PIR VCC
└── LDR VCC

GND
├── OLED GND
├── PIR GND
└── LDR GND

A4 → OLED SDA

A5 → OLED SCL

D2 → PIR OUT

A0 → LDR AO
```

## Folder Structure

```
Smart Study Space Monitor
│
├── docs/
│   ├── architecture.md
│   ├── milestones.md
│   ├── wiring-diagram.png
│   ├── screenshots/
│   └── demo.gif
│
├── include/
│
├── lib/
│   ├── DisplayManager/
│   ├── SensorManager/
│   ├── FocusTimer/
│   ├── AIEngine/
│   └── Utils/
│
├── src/
│   ├── main.cpp
│   ├── display.cpp
│   ├── display.h
│   ├── sensors.cpp
│   ├── sensors.h
│   ├── timer.cpp
│   ├── timer.h
│   ├── ui.cpp
│   └── ui.h
│
├── test/
│
├── platformio.ini
├── README.md
├── LICENSE
└── .gitignore
```

## Plan to implement this project

```
Stage 1
OLED only ✅
(Currently working)

↓

Stage 2
OLED + PIR

↓

Stage 3
OLED + LDR

↓

Stage 4
OLED + Push Button

↓

Stage 5
OLED + Buzzer

↓

Stage 6
Combine everything

↓

Stage 7
Study Session Logic

↓

Stage 8
Arduino Cloud / AppLab

↓

Stage 9
AI Recommendation System

↓

Stage 10
Final Housing + Presentation
```