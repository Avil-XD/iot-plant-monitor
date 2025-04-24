import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import tensorflow as tf

# Load and prepare the data
print("Loading data...")
df = pd.read_csv('data/tinyml_plant_dataset.csv')

# Create features
X = df[['Temperature', 'Humidity', 'Light', 'SoilMoisturePercent']].values
y = (df['SoilMoisturePercent'] < 30).astype(int).values  # Needs water if moisture < 30%

# Split the data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Scale the features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Create a small model suitable for microcontroller
model = tf.keras.Sequential([
    tf.keras.layers.Dense(8, activation='relu', input_shape=(4,)),
    tf.keras.layers.Dense(4, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

# Compile model
model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy'])

print("\nModel Summary:")
model.summary()

# Train the model
print("\nTraining model...")
history = model.fit(X_train_scaled, y_train,
                   epochs=50,
                   batch_size=32,
                   validation_split=0.2,
                   verbose=1)

# Evaluate the model
test_loss, test_accuracy = model.evaluate(X_test_scaled, y_test, verbose=0)
print(f"\nTest accuracy: {test_accuracy:.4f}")

# Convert to TensorFlow Lite
print("\nConverting to TensorFlow Lite...")
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_types = [tf.float16]
tflite_model = converter.convert()

# Save the TF Lite model
with open('model.tflite', 'wb') as f:
    f.write(tflite_model)

# Create a C array for the model
with open('model.h', 'w') as f:
    f.write("const unsigned char model[] = {\n")
    for i, byte in enumerate(tflite_model):
        f.write(f"0x{byte:02x},")
        if (i + 1) % 12 == 0:
            f.write("\n")
    f.write("};\n")
    f.write(f"const unsigned int model_len = {len(tflite_model)};\n")

# Save the scaler parameters
scaler_mean = scaler.mean_
scaler_scale = scaler.scale_

with open('scaler.h', 'w') as f:
    f.write("// Scaler parameters for standardization\n")
    f.write("const float scaler_mean[] = {")
    f.write(", ".join([f"{x:.6f}f" for x in scaler_mean]))
    f.write("};\n\n")
    
    f.write("const float scaler_scale[] = {")
    f.write(", ".join([f"{x:.6f}f" for x in scaler_scale]))
    f.write("};\n")

print("\nFiles generated:")
print("1. model.tflite - TensorFlow Lite model")
print("2. model.h - Model as C array")
print("3. scaler.h - Scaling parameters")