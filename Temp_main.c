// Temperature-Based Fan Control System
// Hardware: ESP8266 + DHT11 + Transistor-controlled Fan + Blynk

#include <DHT.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// Pin definitions
#define DHTPIN 2              // Pin connected to DHT sensor
#define FAN_PIN 5             // PWM pin connected to transistor controlling the fan
#define DHTTYPE DHT11         // Sensor type: DHT11 or DHT22
#define TEMPERATURE_THRESHOLD 25  // Temperature threshold in Celsius

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Blynk Authentication Token & Wi-Fi credentials
char auth[] = "Your_Blynk_Auth_Token";   // Replace with your Blynk Auth Token
char ssid[] = "Your_WiFi_SSID";          // Replace with your Wi-Fi SSID
char pass[] = "Your_WiFi_Password";      // Replace with your Wi-Fi Password

void setup() {
  Serial.begin(9600);

  // Connect to WiFi and Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize DHT sensor
  dht.begin();

  // Set fan pin as output
  pinMode(FAN_PIN, OUTPUT);

  Serial.println("Temperature-Based Fan Control System Started");
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read temperature in Celsius
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }

  int fanSpeed;

  // If temperature is above threshold, map it to PWM value
  if (temperature > TEMPERATURE_THRESHOLD) {
    fanSpeed = map(temperature, TEMPERATURE_THRESHOLD, 40, 100, 255);
    fanSpeed = constrain(fanSpeed, 100, 255); // Ensure minimum speed to avoid stalling
  } else {
    fanSpeed = 0; // Turn off fan if below threshold
  }

  // Write PWM value to fan
  analogWrite(FAN_PIN, fanSpeed);

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(fanSpeed);

  // Send data to Blynk
  Blynk.virtualWrite(V1, temperature);  // Send temperature to Virtual Pin V1
  Blynk.virtualWrite(V2, fanSpeed);     // Send fan speed to Virtual Pin V2

  delay(1000); // Update every second
}

