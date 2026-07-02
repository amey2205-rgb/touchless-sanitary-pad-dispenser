# Touchless Sanitary Pad Dispensing System

An IoT-based automated sanitary pad dispenser with contactless 
activation, digital payment integration, and real-time stock monitoring.

## Demo

### Machine (Internal View)
<img width="694" height="1024" alt="Machine" src="https://github.com/user-attachments/assets/af9a61ce-7d4a-4068-ad51-e287e06539d2" />

### Electronics (ESP32 Controller)
<img width="767" height="1016" alt="Electronics" src="https://github.com/user-attachments/assets/7756f3a0-41fa-4309-8364-595d73fd7de5" />

### Payment Verification — Razorpay Dashboard
<img width="1280" height="719" alt="RazorPay" src="https://github.com/user-attachments/assets/3a15f6b2-9f28-4ec7-843c-9843cce4a931" />

### Backend Webhook Logs
<img width="1280" height="720" alt="Logs" src="https://github.com/user-attachments/assets/89b4698e-2960-4a09-9cd6-ab1f4663b4cb" />


## Features
- Touchless IR sensor-based activation for hygienic operation
- Multi-mode payment: UPI/QR via Razorpay + coin acceptance
- Real-time WiFi-based payment verification before dispensing
- Automated stock monitoring using IR/load-cell sensor
- DC-motor-driven spring mechanism for single-pad dispensing
- LCD display for real-time status feedback

## Hardware Used
| Component | Purpose |
|---|---|
| ESP32 | Main controller |
| IR Sensor | Touchless user detection |
| DC Motor + L298N Driver | Spring dispensing mechanism |
| LCD Display (I2C) | Status display |
| Push Button | Manual override |
| Coin Acceptor | Coin-based payment |

## Tech Stack
- Embedded C (Arduino framework)
- Arduino IDE + ESP32 Board Package
- Razorpay Payment API
- WiFi / HTTP Protocol

## System Flow
User Hand → IR Sensor → ESP32 → QR Payment (Razorpay)
→ WiFi Verification → Stock Check → Motor Driver
→ Dispense Pad → LCD Feedback

## Circuit Diagram
<img width="1131" height="759" alt="circuit" src="https://github.com/user-attachments/assets/8210b42f-5feb-4288-aa9a-31b3b188affd" />

## How to Run
1. Clone this repo
2. Open in Arduino IDE
3. Install ESP32 board package
4. Configure WiFi credentials and Razorpay API keys in `config.h`
5. Flash to ESP32

## Team
Developed as part of SY ENTC project at  
Vishwakarma Institute of Technology, Pune (2025)
