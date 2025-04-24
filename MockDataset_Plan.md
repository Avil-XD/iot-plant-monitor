# Mock Dataset Plan for Plant Water Prediction

## Data Structure
The mock dataset will simulate 1 week of sensor readings with entries every 30 minutes, containing:

1. Timestamp
2. Temperature (°C)
3. Humidity (%)
4. Light (0=Bright, 1=Dark)
5. Soil Moisture Raw (0-1023)
6. Soil Moisture Percent (0-100%)
7. Needs Water (1=Yes, 0=No)

## Value Ranges
- Temperature: 20-35°C
- Humidity: 40-80%
- Light: Day/Night cycles
- Soil Moisture Raw: 300-1023
- Soil Moisture Percent: 0-100%

## Data Patterns to Include
1. Normal Daily Cycles
   - Higher temperatures during day (25-35°C)
   - Lower temperatures at night (20-25°C)
   - Higher humidity at night/early morning
   - Light changes based on time

2. Watering Events
   - Soil moisture spikes after watering
   - Gradual decrease over time
   - "Needs Water" triggers when moisture < 30%

3. Environmental Variations
   - Simulate hot/dry days
   - Simulate humid/cool days
   - Random variations within realistic ranges

## Data Generation Rules
1. Generate timestamps from April 23-30, 2024
2. Create realistic transitions between readings
3. Include some sensor noise/variation
4. Label "Needs Water" based on:
   - Soil moisture < 30%
   - Temperature > 28°C
   - Time since last watering

## Implementation Steps
1. Switch to Code mode
2. Create a Python script to generate the dataset
3. Save as CSV in the data directory
4. Create about 336 entries (7 days × 48 readings per day)