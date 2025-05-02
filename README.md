# 🍎 Smart Fruit Counter – Arduino-based Sorting System

An intelligent **fruit classification and counting system** powered by **Arduino**, featuring:

- ✅ Automatic detection of fruits via ultrasonic sensor
- 🎨 Color detection for **ripeness classification**
- ⚖️ Load cell measurement for **weight-based sorting**
- 🎯 Servo-driven mechanical gates
- 🖥️ Real-time status on **I2C LCD display**
- 🚥 LED indicator for sorting feedback

---

## 📁 Project Structure

```

Fruit-Counter/
├── hardware 
│   ├── fruit-counter-circuit_bb.png
│   └── fruit-counter-circuit.fzz
├── LICENSE
├── README.md
└── software
    ├── calibration
    │   ├── color_calibration.ino
    │   └── weight_calibration.ino
    ├── fruit-counter.ino
    ├── library
    │   ├── arduino-lib-hc-sr04-master.zip
    │   ├── Arduino-LiquidCrystal-I2C-library-master.zip
    │   ├── button-master.zip
    │   └── HX711-master.zip
    └── testing
        ├── button
        │   └── button.ino
        ├── color
        │   └── color.ino
        ├── lcd
        │   └── lcd.ino
        ├── led
        │   └── led.ino
        ├── loadcell
        │   └── loadcell.ino
        ├── servo
        │   └── servo.ino
        └── ultrasonic
            └── ultrasonic.ino

```

---

## 🛠️ Hardware Requirements

| Component                 | Function                                  |
|--------------------------|-------------------------------------------|
| Arduino Uno / Nano       | Main microcontroller                      |
| HX711 + Load Cell        | Measure fruit weight                      |
| TCS3200 Color Sensor     | Detect ripeness (based on RGB levels)     |
| Ultrasonic Sensor x2     | Fruit presence detection (in & out)       |
| Servo Motor x2           | Mechanism for sorting ripe & heavy fruits |
| I2C LCD 16x2             | Display counts and statuses               |
| Relay/MOSFET (optional)  | Conveyor belt control (digital pin)       |
| LED                      | Status indication                         |

---

## 🔌 Pin Mapping

| Arduino Pin | Connected To             | Description                          |
|-------------|--------------------------|--------------------------------------|
| A0          | HX711 SCK                | Load cell clock                      |
| A1          | HX711 DOUT               | Load cell data                       |
| A2          | Conveyor Relay/MOSFET    | Conveyor motor control               |
| 2,3         | Ultrasonic Sensor 1      | Detects incoming fruit               |
| 4–7         | TCS3200 S0–S3            | Frequency scaling and color filters |
| 8           | TCS3200 OUT              | Pulse output from color sensor       |
| 9           | Servo 1                  | Sorting based on ripeness            |
| 10          | Servo 2                  | Sorting based on weight              |
| 11,12       | Ultrasonic Sensor 2      | Detects fruit on weighing platform   |
| 13          | LED                      | Feedback during sorting              |
| A4, A5      | I2C LCD                  | SDA and SCL lines                    |

---

## 📋 Features

- **Automatic Detection**:
  - When a fruit is detected (Ultrasonic 1), conveyor pauses
- **Color Classification**:
  - Uses RGB thresholds to classify **ripe** vs **unripe**
  - If not ripe, Servo 1 diverts the fruit
- **Weight Classification**:
  - Uses load cell (HX711) to weigh fruit
  - If weight < threshold → **small fruit**
  - If weight ≥ threshold → **big fruit**
  - Servo 2 diverts accordingly
- **Display Output**:
  - LCD shows:
    - Total fruits processed (`T`)
    - Ripe fruits counted (`R`)
    - Small (`S`) and Big (`B`) counts
- **Visual Debugging**:
  - Serial Monitor logs distance, color, weight, and action

---

## 🧪 Calibration

### 1. Color Calibration

Use `color_calibration.ino` to get red/green/blue values:

```cpp
int redtopSP = ...;
int redbotSP = ...;
int greentopSP = ...;
int greenbotSP = ...;
int bluetopSP = ...;
int bluebotSP = ...;
```

### 2. Load Cell Calibration

Use `weight_calibration.ino` to find scale factor and update:

```cpp
float calibrationValue = ...;
```

---

## 📊 Sample Serial Output

```txt
Distance1: 14
Total: 3
Red: 39 | Green: 23 | Blue: 19
Ripe Counter: 2
Distance2: 8
Weight: 192 g
S Fruit: 2
```

---

## 📦 Libraries Required

Install these libraries manually or through Library Manager:

* **HX711\_ADC**
* **LiquidCrystal\_I2C**
* **Servo**
* **TCS3200** (manually implement if needed)
* **Ultrasonic** (optional helper library)

All `.zip` libraries are located in `/software/library/`.

---

## 🧠 System Flow Summary

1. Fruit detected at **Sensor 1** → Pause conveyor
2. Read **color** → If unripe, divert via **Servo 1**
3. Fruit detected at **Sensor 2** → Weigh fruit
4. Read **weight** → Sort using **Servo 2**
5. Update counters on **LCD**
6. Resume conveyor

---

## 👨‍💻 Author

Created by **2black0 (Ardy Seto)**
🚀 Project for embedded systems and mechatronics learning

---

## 📜 License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.