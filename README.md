
# 🛠️ LLM-Powered Educational Platform for Smart Manufacturing

**Bridging the gap between students and Human-Centric Systems**

[![ASEE Make It!](https://img.shields.io/badge/ASEE_Make_It!-Manufacturing_Division-blue?style=for-the-badge)](#)
[![Python](https://img.shields.io/badge/Python-3.8%2B-green?style=for-the-badge&logo=python)](#)
[![Arduino](https://img.shields.io/badge/Arduino-IoT-00979D?style=for-the-badge&logo=arduino)](#)
[![Llama3](https://img.shields.io/badge/Llama_3-Fine--Tuned-orange?style=for-the-badge)](#)

</div>

---

## 📖 Project Overview

This work is an interactive, AI-driven educational platform designed to accelerate learning in smart manufacturing, specifically tailored for 3D printing systems. In the era of **Industry 4.0 and 5.0**, understanding physical machine behaviors, anomaly detection, and troubleshooting protocols is paramount. 

Standard Large Language Models (LLMs) often hallucinate when asked about specific hardware states. **This project solves this** by integrating a domain-specific, fine-tuned **Llama 3** model with **real-time hardware telemetry** and Retrieval-Augmented Generation (RAG). By capturing live sensor data and combining it with expert knowledge, the platform provides engineering students with highly accurate, context-aware troubleshooting assistance.

---

## ✨ Key Features

- 🧠 **Domain-Specific AI:** Features a locally run Llama 3 model, fine-tuned on custom manufacturing datasets to prevent hallucinations and provide expert-level diagnostic feedback.
- 📊 **Real-Time Telemetry Dashboard:** A Python-based GUI that visualizes live machine states, including nozzle/bed temperatures, XYZ acceleration, and motor currents.
- 📡 **IoT Data Acquisition:** Custom Arduino architecture reading directly from embedded sensors on a Sovol SV04 3D printer.
- 🗣️ **Accessible & Human-Centric:** Incorporates Text-to-Speech (TTS) capabilities to democratize manufacturing education and improve student-machine engagement.
- 📚 **RAG Integration:** Pulls from machine-specific manuals and troubleshooting protocols for highly relevant answers.

---

## 📂 Repository Structure

| File | Description | Role |
| :--- | :--- | :--- |
| 🗄️ `FinalDataset.json` | JSON dataset containing specific 3D printing process parameters paired with expert QA. | **Data** (Used for LLM fine-tuning) |
| 📓 `LLM_Fine_Tuning_Llama.ipynb` | Jupyter Notebook utilizing `unsloth` to perform parameter-efficient fine-tuning (PEFT/LoRA) on the Llama 3 model. | **AI/ML Model Training** |
| 🐍 `PlatformLLM TTS RAG2.py` | The main application file. Runs the Tkinter/Matplotlib dashboard, processes serial data, and handles the LLM/RAG/TTS pipeline. | **Core Application / GUI** |
| 🔌 `ArduinoBI2.ino` | C++ firmware for the Arduino. Captures data from DHT22, MAX31856, accelerometers, and ACS712 current sensors, and broadcasts via Serial. | **IoT Data Acquisition** |
| 📜 `AI-Powered Manufacturing Education Poster` | Design outlines and abstracts for the ASEE Make It! Manufacturing Division Poster Session. | **Documentation / Presentation** |

---

## ⚙️ Hardware Requirements

To replicate the physical twin setup, the following hardware is required:
* **3D Printer:** Sovol SV04 (or adaptable to other Cartesian/CoreXY systems)
* **Microcontroller:** Arduino Mega 2560 (or equivalent)
* **Sensors:**
    * `DHT22` (Ambient Temperature & Humidity)
    * `Adafruit MAX31856` / `MLX90614` (Nozzle & Bed Thermocouples)
    * `ACS712` (Current sensors for X, Y, Z, and Filament motors)
    * `Accelerometer` (XYZ Kinematic Tracking)
    * `Laser-based sensor` (Filament presence detection)

---

## 🚀 Setup & Installation

### 1. Hardware Setup
Flash `ArduinoBI2.ino` to your Arduino board. Ensure sensors are wired correctly according to the pinout definitions in the code (e.g., `DHTPIN 2`, `MAX31856_CS_PIN 53`).

### 2. Environment Setup
Install the required Python dependencies for the dashboard and AI platform:
Code output
README.md generated successfully.

```bash
pip install pyserial matplotlib pillow pandas numpy gTTS pygame torch langchain_ollama

> **Note:** Ensure Ollama is installed and running locally with the necessary base model.

### 3. Model Fine-Tuning (Optional)
If you wish to re-train the model on new data:
1. Open `LLM_Fine_Tuning_Llama.ipynb` in Google Colab or a local Jupyter environment.
2. Ensure you have a CUDA-capable GPU.
3. Run the notebook to fine-tune the model using `FinalDataset.json` and export to your local Ollama instance.

## 🖥️ Usage

1. Power on the 3D printer and connect the Arduino via USB to your host computer.
2. Launch the Dashboard.
3. Select your designated COM port from the GUI dropdown and click **Start capturing data**.
4. Use the interface to monitor live graphs. Type or speak queries into the chatbot to receive real-time, context-aware troubleshooting advice!

## 🎓 Academic Context

This project was developed for presentation at the **ASEE Make It! Manufacturing Division Poster Session**. The platform is designed not just as a tool, but as a modular educational framework that can adapt to CNCs, robotic arms, or any smart manufacturing node to foster next-generation engineering skills.
