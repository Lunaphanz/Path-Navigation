# 📌 Path Navigation Robot – CSE 325 Project 5

This project implements an autonomous path-navigating robot using the **FRDM-KL46Z** board.  
The robot can:

- Follow maze paths using **line sensors**  
- Detect tile colors using a **TCS34725 RGB Color Sensor**  
- Apply a color-mapping rule (Blue→Red, Yellow→Red, Red→Green, Green→Blue)  
- Determine its **finish color** based on the starting tile  
- Navigate through the maze using modular wheel + sensor drivers  

This repository contains all embedded drivers and high-level logic used to control the robot.

---

# 📁 Directory Overview

```
Project_5/
├── Project_5.c          → Main program (setup, start tile, finish logic, navigation loop 
│
├── color_sensor.c       → High-level color classification + normalization
│
├── TCS34725_I2C.c       → Low-level TCS34725 I2C register driver // use I2C_outline.txt for this
│
├── line_sensor.c        → Line sensor ADC readings // follow ADC_lightsensor.c
│
├── wheel.c              → Motor PWM control (forward/turn/stop)
│
├── mtb.c                → Micro trace buffer
├── semihost_hardfault.c → Debug crash handler
│
└── README.md            → This file
```

---

# 🎨 Color Sensor System (TCS34725 + color_sensor.c)

The color sensor module is composed of **two layers**:

---

## 1️⃣ Low-Level TCS34725 Driver — `TCS34725_I2C.c`

Implements all direct communication with the TCS34725 sensor:

- I2C start/write/read operations  
- Writing PON + AEN bits to enable sensor  
- Reading 16-bit RGBC data registers  
- Integration time and configuration settings  
- Command register formatting (`0x80 | reg`)

## 2️⃣ High-Level Color Logic — `color_sensor.c`

---

# ⚫ Line Sensor System — `line_sensor.c`

Two IR reflectance sensors measure ADC values to track lines:

- Black line → low ADC  
- White floor → high ADC  

Supports:

- Continuous left/right ADC readings  
- Threshold comparison  
- Line-following corrections (turn left/right vs. straight)

ADC pin routing and channel configuration are auto-generated in:

```
peripherals.c  
pin_mux.c
```

---

# 🚗 Wheel Driver — `wheel.c`

Controls both drive motors through **TPM PWM**:

- `go_straight()`  
- `turn_left45()`  
- `turn_right45()`  
- `stop()`  

PWM signals and TPM channel assignments are configured via:

```
peripherals.c  
pin_mux.c
```

---

# 🤖 Maze Navigation Logic — `function.c`

High-level control logic:

| Function | Purpose |
|----------|---------|
| `keep_going()` | Uses line sensors to decide turns or straight movement |
| `check_finish()` | Multi-sample finish tile verification |
| `get_out()` | Initial entry movement |
| `get_in()` | Final movement once finish tile is reached |

The main navigation loop continuously calls:

```c
keep_going();
finish_reached = check_finish();
```

until the final tile is confirmed.

---

# 🧠 Main Application — `Project_5.c`

Handles:

1. Board + peripheral initialization  
2. Sensor and motor setup  
3. Starting tile detection (`get_color()`)  
4. Finish tile computation (`map_color()`)  
5. Wait for SW2 press  
6. Maze navigation via line sensing  
7. Color-based finishing detection  

Uses `PRINTF()` for debugging values and state output.

---

# 🛠 Pin Configuration

All hardware pin assignments—including:

- I2C for TCS34725  
- ADC for line sensors  
- TPM PWM for motors  
- GPIO for switches  

are defined in:

```
board/pin_mux.c  
peripherals.c
```

These files reflect the exact electrical wiring on your KL46Z board.

---

# 🏁 Summary

This project integrates:

- TCS34725 color sensing  
- Line following via ADC  
- Motor control via TPM  
- A modular driver architecture  
- Stable color-based maze completion logic  

The structure separates sensor drivers, motor control, and navigation logic for clarity and maintainability.
