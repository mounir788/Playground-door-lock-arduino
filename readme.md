# 🚪🔒 Smart Playground Door Lock — Arduino & ESP

This repository contains the code for the **Smart Playground Door Lock** used in my graduation project. The system controls a servo motor door mechanism, keypad password entry, LCD display, and communicates via an ESP module for remote access and monitoring.

---

## 📌 Project Overview

- **Purpose:** Secure playground access with password-protected door lock.
- **Hardware:** Arduino Uno, ESP module, 16x2 LCD, micro servo (MG90s), keypad, limit switch.
- **Features:**
  - Password entry with keypad
  - Servo motor opens/closes door
  - LCD shows instructions, status, and countdown timer
  - Limit switch resets system when door closes
  - ESP module handles Wi-Fi and API communication

---

## 📂 Repository Structure

```
📁 /arduino-code
   ├─ Arduino.ino      # Main Arduino code
📁 /esp-code
   ├─ ESP8266.ino     # ESP module code for Wi-Fi & API
```

---

## ⚙️ How It Works

1. User enters password on the keypad.
2. If correct, servo motor opens the door.
3. LCD displays countdown timer for door open time.
4. Limit switch detects when the door closes and resets the system.
5. ESP module sends/receives data to/from the web dashboard via serial communication.

---

## 🛠️ Requirements

- Arduino IDE
- Arduino Uno
- ESP8266 / ESP32
- 16x2 LCD
- Keypad
- Micro Servo (MG90s)
- Push Button (limit switch)
- Power source (batteries)

---

## 🚀 How to Upload

1. Open `Arduino.ino` in the Arduino IDE and upload it to the Arduino Uno.
2. Open `ESP8266.ino` in the Arduino IDE and upload it to the ESP module.
3. Connect the Arduino and ESP via serial communication as described in your circuit.

---

## ✅ License

This project is for educational purposes. You can adapt or reuse parts with credit.

