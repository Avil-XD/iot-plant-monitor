import pandas as pd
import numpy as np
from datetime import datetime, timedelta

np.random.seed(42)

# Generate timestamps for 60 days with readings every 30 minutes
start_date = datetime(2024, 2, 1)
dates = [start_date + timedelta(minutes=30 * x) for x in range(2880)]  # 60 days * 48 readings

# Initialize lists
temperatures = []
humidities = []
lights = []
soil_moistures_percent = []
needs_water = []

# Simulation parameters
last_watering = start_date - timedelta(hours=12)
current_moisture = 180

for i, date in enumerate(dates):
    day = (date - start_date).days
    hour = date.hour

    # Simulate 4 climate patterns every 15 days
    if day < 15:
        # Normal spring-like Assam weather
        temp_range = (26, 33)
        humidity_range = (70, 85)
        moisture_loss_range = (0.5, 1.2)
    elif day < 30:
        # Hot and dry spell
        temp_range = (30, 36)
        humidity_range = (55, 70)
        moisture_loss_range = (1.0, 2.5)
    elif day < 45:
        # Cloudy and humid days (low light, high humidity)
        temp_range = (24, 30)
        humidity_range = (75, 90)
        moisture_loss_range = (0.3, 1.0)
    else:
        # Rainy week (frequent watering-like events)
        temp_range = (23, 29)
        humidity_range = (80, 95)
        moisture_loss_range = (0.2, 0.6)

    # Temperature with diurnal variation
    base_temp = (temp_range[0] + temp_range[1]) / 2 + 4 * np.sin((hour - 12) * np.pi / 12)
    temp = np.clip(base_temp + np.random.normal(0, 1.5), *temp_range)

    # Humidity inversely related to temp
    base_humidity = (humidity_range[0] + humidity_range[1]) / 2 - 5 * np.sin((hour - 12) * np.pi / 12)
    humidity = np.clip(base_humidity + np.random.normal(0, 5), *humidity_range)

    # Light condition: 0 = day, 1 = night
    light = 1 if (hour < 6 or hour > 18) else 0

    # Moisture loss simulation
    moisture_loss = np.random.uniform(*moisture_loss_range)
    current_moisture -= moisture_loss

    # Watering condition
    if current_moisture < 45 and (date - last_watering).total_seconds() > 8 * 3600:
        current_moisture = np.random.uniform(170, 200)
        last_watering = date

    current_moisture = np.clip(current_moisture, 30, 200)

    # Map to % from raw
    moisture_percent = int((current_moisture - 31) / (200 - 31) * 50 + 50)
    moisture_percent = np.clip(moisture_percent, 30, 100)

    # Labeling logic
    if moisture_percent < 50:
        needs_water_flag = 1
    elif 50 <= moisture_percent < 60:
        needs_water_flag = 1 if temp > 33 or humidity < 65 else 0
    else:
        needs_water_flag = 0

    # Append
    temperatures.append(round(temp, 2))
    humidities.append(round(humidity, 2))
    lights.append(light)
    soil_moistures_percent.append(moisture_percent)
    needs_water.append(needs_water_flag)

# Save as DataFrame
df = pd.DataFrame({
    'Timestamp': dates,
    'Temperature': temperatures,
    'Humidity': humidities,
    'Light': lights,
    'SoilMoisturePercent': soil_moistures_percent,
    'NeedsWater': needs_water
})

# Save to CSV
df.to_csv('data/tinyml_plant_dataset.csv', index=False)
print(f"TinyML-ready dataset generated with {len(df)} entries.")

# Show quick view
print("\nSample data:")
print(df.head())
print("\nSummary statistics:")
print(df.describe())
