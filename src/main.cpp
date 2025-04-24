#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <LittleFS.h>
#include <time.h>
#include "tinyml_inference.h"

// WiFi credentials
const char* ssid = "TSranger";
const char* password = "#Ts242002";

// Pin definitions
#define DHTPIN D4
#define DHTTYPE DHT22
#define LDR_PIN D5
#define SOIL_PIN A0

// Global objects
ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

// Data logging configuration
const char* dataFile = "/sensor_data.csv";
const unsigned long loggingInterval = 30000; // Log every 30 seconds
unsigned long lastLog = 0;

void logData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int ldrValue = digitalRead(LDR_PIN);
    int soilValue = analogRead(SOIL_PIN);
    int soilMoisturePercent = map(soilValue, 1023, 300, 0, 100);
    soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);
    
    // Get watering prediction
    float wateringPrediction = predictWateringNeed(
        temperature,
        humidity,
        static_cast<float>(ldrValue),
        static_cast<float>(soilMoisturePercent)
    );
    
    time_t now = time(nullptr);
    String timestamp = String(ctime(&now));
    timestamp.trim();  // Remove trailing newline
    
    String dataString = timestamp + "," +
                       String(temperature) + "," +
                       String(humidity) + "," +
                       String(ldrValue) + "," +
                       String(soilValue) + "," +
                       String(soilMoisturePercent) + "," +
                       String(wateringPrediction, 3) + "\n";

    File file = LittleFS.open(dataFile, "a");
    if (file) {
        file.print(dataString);
        file.close();
        Serial.println("Data logged: " + dataString);
    }
}

void handleRoot() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int ldrValue = digitalRead(LDR_PIN);
    int soilValue = analogRead(SOIL_PIN);
    int soilMoisturePercent = map(soilValue, 1023, 300, 0, 100);
    soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

    // Get watering prediction
    float wateringPrediction = predictWateringNeed(
        temperature,
        humidity,
        static_cast<float>(ldrValue),
        static_cast<float>(soilMoisturePercent)
    );

    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta http-equiv='refresh' content='5'>";
    html += "<title>ESP8266 Plant Monitor</title></head>";
    html += "<body style='font-family:Arial;text-align:center;'>";
    html += "<h1>Plant Monitor with ML</h1>";
    html += "<p><b>Temperature:</b> " + String(temperature) + " °C</p>";
    html += "<p><b>Humidity:</b> " + String(humidity) + " %</p>";
    html += "<p><b>Light Intensity:</b> " + String(ldrValue == 0 ? "Bright" : "Dark") + "</p>";
    html += "<p><b>Soil Moisture:</b> " + String(soilMoisturePercent) + " %</p>";
    html += "<p><b>Soil Moisture (Raw):</b> " + String(soilValue) + "</p>";
    html += "<p style='font-weight:bold;color:" + String(wateringPrediction > 0.5 ? "red" : "green") + ";'>";
    html += "Watering Status: " + String(wateringPrediction > 0.5 ? "Needs Water!" : "Water Level OK") + "</p>";
    html += "<hr><p><a href='/data'>Download Sensor Data</a></p>";
    html += "<p><a href='/clear'>Clear Data Log</a></p>";
    html += "<p style='color:gray;font-size:12px;'>Page refreshes every 5 seconds</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleData() {
    File file = LittleFS.open(dataFile, "r");
    if (!file) {
        server.send(404, "text/plain", "No data found");
        return;
    }

    server.setContentLength(file.size());
    server.send(200, "text/csv", "");
    
    const size_t bufSize = 1024;
    uint8_t buf[bufSize];
    while (file.available()) {
        size_t len = file.read(buf, bufSize);
        server.sendContent((char*)buf, len);
    }
    file.close();
}

void handleClear() {
    if (LittleFS.remove(dataFile)) {
        File file = LittleFS.open(dataFile, "w");
        if (file) {
            // Write CSV header
            file.println("Timestamp,Temperature,Humidity,Light,SoilRaw,SoilPercent");
            file.close();
            server.send(200, "text/plain", "Data log cleared and initialized");
        } else {
            server.send(500, "text/plain", "Error creating new file");
        }
    } else {
        server.send(500, "text/plain", "Error clearing data");
    }
}

void setup() {
    Serial.begin(9600);
    
    // Initialize TinyML
    if (!setupTinyML()) {
        Serial.println("Failed to initialize TinyML!");
        return;
    }
    Serial.println("TinyML initialized successfully");
    
    // Initialize sensors
    dht.begin();
    pinMode(LDR_PIN, INPUT);
    pinMode(SOIL_PIN, INPUT);

    // Initialize filesystem
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }

    // Create data file with header if it doesn't exist
    if (!LittleFS.exists(dataFile)) {
        File file = LittleFS.open(dataFile, "w");
        if (file) {
            file.println("Timestamp,Temperature,Humidity,Light,SoilRaw,SoilPercent,WateringPrediction");
            file.close();
        }
    }

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure time
    configTime(5.5 * 3600, 0, "pool.ntp.org"); // IST timezone offset

    // Setup server routes
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/clear", handleClear);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();

    // Log data every 5 minutes
    unsigned long currentMillis = millis();
    if (currentMillis - lastLog >= loggingInterval) {
        logData();
        lastLog = currentMillis;
    }
}