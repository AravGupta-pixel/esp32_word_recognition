 **ESP32 Offline Voice Recognition (ON / OFF)**

 
An end-to-end **embedded machine learning system** that runs on an **ESP32** and recognizes voice commands (“ON” / “OFF”) using an **INMP441 I2S microphone**, enabling real-time offline device control without cloud dependency.

*Project Overview*

This project implements a **TinyML-based keyword spotting system** that runs completely on-device. The system listens for voice commands and triggers actions instantly, without relying on Wi-Fi, cloud APIs, or external servers.

 
 **Key Features**

*  Fully offline voice recognition
*  Keyword detection: `"ON"` / `"OFF"`
*  Real-time inference on ESP32
*  No cloud dependency
*  Custom dataset recorded from actual hardware mic
*  End-to-end ML pipeline (data → training → deployment)

 **Hardware Used**

* ESP32 Dev Board
* INMP441 I2S Microphone
* Jumper wires
* USB cable

---

 System Architecture

Microphone (INMP441)
        ↓
ESP32 (I2S Audio Capture)
        ↓
Serial Streaming (WAV Data)
        ↓
Python Script (Dataset Collection)
        ↓
Google Colab (Model Training - CNN / MFCC)
        ↓
TFLite Model
        ↓
ESP32 Deployment
        ↓
Real-time Voice Control

 **Tech Stack**

* ESP32 (Arduino Framework)
* I2S Audio Protocol
* Python (PySerial, Wave)
* Google Colab
* TensorFlow / TFLite
* MFCC Feature Extraction

---

 Dataset Collection Pipeline

Audio samples were recorded directly from the ESP32 using a custom serial-based WAV streaming system.

Collected classes:

* `"ON"` → 50+ samples
* `"OFF"` → 50+ samples
* `"Noise"` → background audio

Each sample is stored as a `.wav` file for training.

---

 **Model Training**

The model was trained using:

* MFCC feature extraction
* CNN-based classifier
* TensorFlow/Keras in Google Colab
* Converted to TensorFlow Lite for microcontroller deployment

---
**Challenges & Fixes**

### 1. INMP441 32-bit I2S Audio Issue

* Mic outputs 32-bit frames
* Required correct shifting (`>> 11`) to extract valid signal

### 2. Silent Audio Problem

* Initial recordings were extremely quiet
* Fixed using software gain adjustment and correct I2S configuration

### 3. Serial Streaming Instability

* Early versions dropped audio packets
* Fixed using chunk-based buffering and baud rate optimization (921600)

### 4. Edge Impulse Mismatch (Optional Phase)

* Attempted Edge Impulse pipeline
* Faced pin mapping + firmware mismatch issues
* Switched to custom Colab pipeline for full control

---

Results

* Stable offline keyword detection
* Real-time response on ESP32
* Works fully without internet
* Reliable inference after custom dataset training

---

Future Improvements

* Add more keywords (STOP, START, YES, NO)
* Improve noise robustness
* Add wake-word detection
* Optimize model for ultra-low power mode

---

Project Structure

```
esp32-voice-recognition/
│
├── esp32_code/          # Arduino sketch for audio capture
├── python_scripts/      # Dataset recording scripts
├── dataset/             # WAV audio samples
└── README.md
```

---

 Conclusion

This project demonstrates how **embedded ML + microcontrollers** can be used to build real-world voice-controlled systems without cloud dependency. It also highlights the importance of **hardware-aware dataset collection and signal processing** in TinyML systems.

*this is my first repo and a project so please excuse me for any documentation error* 

