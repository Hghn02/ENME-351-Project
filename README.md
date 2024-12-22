# ENME-351-Project

This was a project for my ENME351 class at the University of Maryland. I created an IMU-guided 2-Axis gimbal that had an IR distance sensor and laser diode mounted to it for target sensing. The software for this project was written in Arduino C++ and Processing.

---

## Hardware List
The following components were used in this project:
- **Arduino Uno**
- **BNO055 9-DOF Absolute Orientation IMU Sensor**
- **GP2Y0A21YK0F SHARP IR Distance Sensor**
- **HiLetgo 5V 650nm 5mW Laser Diode**
- **FEETECH Mini Servo FT1117M (x2)**
- **6.0 V, 2200 mAh Rechargeable NiMH Battery (x2)**
- **Breadboard**
- **M3 screws**

---

## How It Works

The IMU-guided 2-Axis gimbal uses a BNO055 9 DOF IMU sensor to detect Euler angles, which are transmitted to the Arduino via I2C. The Arduino processes the IMU data and controls two FEETECH Mini Servos, which adjust the gimbal's pan and tilt angles.
Mounted on the gimbal, the IR distance sensor detects the target's range, while the laser diode pulses based on the distance sensed. The laser diode is controlled through an NPN transistor circuit. The servo motors are powered by two 6.0V rechargeable NiMH batteries, with all components wired through a breadboard.

The accompanying Processing software displays live telemetry data, including orientation angles and distance readings, providing real-time feedback to the user.
---

## Telemetry Window
Here’s a picture of the graphical display window

<img src="https://github.com/Hghn02/ENME-351-Project/blob/df173d63fadcd1a94ad8d79a3953523d8c28310d/Demo_Software/Telemetry/Screenshot%202024-12-07%20133217.png" alt="Screenshot Placeholder" width="650" height="400">

---

## Demo Video
Watch the project demonstration video here:

[![Watch the video](https://img.youtube.com/vi/X9MKAsoJGBA/0.jpg)](https://www.youtube.com/watch?v=X9MKAsoJGBA)

---
