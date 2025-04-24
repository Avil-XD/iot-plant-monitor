# Smart Plant Monitoring System with TinyML

This project implements a smart plant monitoring system using an ESP8266 microcontroller and TinyML. The system monitors plant health using various sensors and provides intelligent watering recommendations based on a trained machine learning model.

## Features

- Real-time monitoring of:
  - Temperature
  - Humidity
  - Light intensity
  - Soil moisture
- TinyML-powered watering recommendations
- Web interface for real-time monitoring
- Data logging to CSV file
- Automated alerts when watering is needed

## Hardware Requirements

- NodeMCU ESP8266
- DHT22 Temperature and Humidity Sensor
- LDR (Light Dependent Resistor)
- Soil Moisture Sensor
- Jumper Wires
- Breadboard

## Pin Connections

- DHT22 -> D4
- LDR -> D5
- Soil Moisture Sensor -> A0

## Software Requirements

- PlatformIO
- Visual Studio Code
- Python 3.x (for training the ML model)
- Required Python packages: tensorflow, pandas, numpy, scikit-learn

## Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/plant-monitoring-system.git
cd plant-monitoring-system
```

2. Open the project in VS Code with PlatformIO

3. Update WiFi credentials in src/main.cpp:
```cpp
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
```

4. Upload the code to your ESP8266 using PlatformIO

## Machine Learning Model

The system uses a TinyML model trained on plant data to make watering recommendations. The model takes into account:
- Temperature
- Humidity
- Light intensity
- Soil moisture

To retrain the model:
1. Prepare your dataset in data/tinyml_plant_dataset.csv
2. Run the training script:
```bash
python train_tinyml_model.py
```

## Web Interface

Access the web interface by navigating to the ESP8266's IP address in your browser. The interface shows:
- Current sensor readings
- Watering recommendations
- Option to download logged data
- Option to clear logs

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.