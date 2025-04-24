# 🌱 Smart Plant Monitor with TinyML

<div align="center">

![Plant Monitor Banner](https://user-images.githubusercontent.com/YourUserID/iot-plant-monitor/main/docs/banner.png)

[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange?logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MCIgaGVpZ2h0PSI0MCIgdmlld0JveD0iMCAwIDI2LjkyNSAyNi45MjUiPjxwYXRoIGQ9Ik0xMy40NjIgMEMxOS44OCAwIDI1LjA5IDUuMjEgMjUuMDkgMTEuNjI4YzAgNC44NC0yLjk3IDguOTgtNy4yIDEwLjczdi0yLjk3YzIuOTctMS4zNCA1LjA2LTQuMzEgNS4wNi03Ljc2IDAtNC43MS0zLjgyLTguNTMtOC41My04LjUzcy04LjUzIDMuODItOC41MyA4LjUzYzAgMy40NSAyLjA5IDYuNDIgNS4wNiA3Ljc2djIuOTdjLTQuMjMtMS43NS03LjItNS44OS03LjItMTAuNzNDMS44MzUgNS4yMSA3LjA0NSAwIDEzLjQ2MiAwaiBtMCAxNi4zOWMtMi42MSAwLTQuNzYtMi4xNS00Ljc2LTQuNzZzMi4xNS00Ljc2IDQuNzYtNC43NiA0Ljc2IDIuMTUgNC43NiA0Ljc2LTIuMTUgNC43Ni00Ljc2IDQuNzZ6IiBmaWxsPSIjZmZmIi8+PC9zdmc+)](https://platformio.org/)
[![ESP8266](https://img.shields.io/badge/ESP8266-IoT-blue?logo=espressif&logoColor=white)](https://www.espressif.com/)
[![TensorFlow Lite](https://img.shields.io/badge/TensorFlow%20Lite-AI-orange?logo=tensorflow)](https://www.tensorflow.org/lite)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

*A smart plant monitoring system that uses TinyML to provide intelligent watering recommendations* 🤖

[Features](#-features) • [Hardware Setup](#-hardware-setup) • [Installation](#-installation) • [Usage](#-usage) • [Machine Learning](#-machine-learning) • [Contributing](#-contributing)

</div>

## 🌟 Features

<div align="center">

| Feature | Description |
|---------|-------------|
| 🌡️ Temperature | Real-time temperature monitoring |
| 💧 Humidity | Accurate humidity tracking |
| ☀️ Light | Ambient light measurement |
| 🪴 Soil Moisture | Precise soil moisture detection |
| 🧠 TinyML | AI-powered watering recommendations |
| 📊 Data Logging | CSV format data storage |
| 🌐 Web Interface | Real-time monitoring dashboard |
| 📱 Mobile Friendly | Responsive design |

</div>

## 🛠️ Hardware Setup

### Components Required

- NodeMCU ESP8266 Board
- DHT22 Temperature & Humidity Sensor
- LDR (Light Dependent Resistor)
- Soil Moisture Sensor
- Jumper Wires
- Breadboard

### Pin Connections

```
┌──────────────┐
│   ESP8266    │
│              │    DHT22 → D4
│  ┌──────┐    │    LDR → D5
│  │      │    │    Soil Sensor → A0
│  └──────┘    │
│              │
└──────────────┘
```

## 🚀 Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Avil-XD/iot-plant-monitor.git
   cd iot-plant-monitor
   ```

2. **Open in VS Code with PlatformIO**
   - Install PlatformIO extension
   - Open project folder
   - Wait for indexing

3. **Configure WiFi**
   ```cpp
   // In src/main.cpp
   const char* ssid = "your_wifi_ssid";
   const char* password = "your_wifi_password";
   ```

4. **Upload to ESP8266**
   - Click ➡️ icon in PlatformIO toolbar
   - Wait for upload completion
   - Monitor serial output for IP address

## 💻 Usage

### Web Interface

Access `http://<esp8266-ip-address>` to view:

- 📊 Real-time sensor readings
- 🚰 Watering recommendations
- 📈 Historical data
- ⚙️ System controls

### Data Logging

- Data is logged every 30 seconds
- Download CSV from web interface
- Format: Timestamp, Temperature, Humidity, Light, Soil Moisture, ML Prediction

## 🧠 Machine Learning

### Training the Model

1. **Prepare Dataset**
   ```bash
   python generate_mock_data.py
   ```

2. **Train Model**
   ```bash
   python train_tinyml_model.py
   ```

3. **Deploy**
   - Model automatically converts to TFLite
   - Generates header files for ESP8266

### Model Architecture

```
Model: Sequential
┌─────────────────┐
│ Dense(8, ReLU)  │
├─────────────────┤
│ Dense(4, ReLU)  │
├─────────────────┤
│ Dense(1, Sigmoid)│
└─────────────────┘
```

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Open pull request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

<div align="center">

### Made with ❤️ by [Avil-XD](https://github.com/Avil-XD)

</div>