# Bluetooth-Controlled Claw Mechanism (Arduino Nano ESP32)

This project implements a wireless claw mechanism controlled over Bluetooth Low Energy (BLE) using an Arduino Nano ESP32. A smartphone sends hex commands through the LightBlue app, and the ESP32 interprets them to drive a high-torque LDX-335MG servo that opens and closes the claw mechanism in real time.

The system demonstrates:
- BLE communication using ESP32
- Real-time command parsing
- PWM-based servo control
- Stable external 7.4V power delivery
- Voltage smoothing using a 1000µF capacitor

---

## Hardware Components

Microcontroller  : Arduino Nano ESP32  
https://www.microcenter.com/product/704595/arduino-nano-esp32-with-headers  

Claw (Mechanical Gripper)  
https://www.amazon.com/dp/B089KMK954  

Servo  : LDX-335MG Metal Gear High Torque Servo  

Breadboard  
https://www.amazon.com/dp/B07LF84HWK  

Capacitor  : 1000µF Electrolytic Capacitor  
https://www.amazon.com/dp/B0CMQCC5D3  

Battery  : 7.4V Battery (Any)

Wires: Male-to-Male Jumper Wires (Any)

---

## Setup Instructions

1) Upload the Code  
- Copy the source code in the **Source_code** section of this repository.  
- Open the project in Arduino IDE.  
- Select board: Arduino Nano ESP32.  
- Upload the code to the board.  

2) Wiring  
Follow the wiring schematic shown in the **Wiring_Diagram** section of this repository:
- Battery (+) → Breadboard + rail  
- Battery (−) → Breadboard − rail  
- Capacitor across + and − rails  
- + rail → Arduino VIN  
- − rail → Arduino GND  
- D5 → Servo Signal (white wire)  
- + rail → Servo VCC (red wire)  
- − rail → Servo GND (black/brown wire)

3) LightBlue App Configuration  
- Download LightBlue from the App Store.  
- Power the Arduino Nano ESP32.  
- Open LightBlue and scan for devices.  
- Connect to: nano_claw  
- Go to the Write section of the BLE characteristic.

---

## Control Commands

Send the following hex values in the Write field:

6F  → Open Claw  
63  → Close Claw  

The ESP32 receives the command and rotates the LDX-335MG servo to predefined angles, actuating the claw mechanism accordingly.

---

## System Flow

Smartphone → BLE → ESP32 → PWM (D5) → LDX-335MG Servo → Claw Movement

## Intended Application

This project will be used to pick up and drop payloads by attaching the claw mechanism to a drone. The Bluetooth control system allows remote actuation of the gripper during flight operations.

---

```
NOTE: 

You do not strictly need a capacitor for this setup. However, we included a 1000µF capacitor across the power rails to stabilize voltage in case the servo draws sudden current spikes. This helps protect the Arduino Nano ESP32 from brownouts or potential damage caused by power fluctuations.
```
