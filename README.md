// Temperature-based-fan-control-system


#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

#define DHTPIN 2           // Pin connected to DHT sensor
#define FAN_PIN 5          // PWM pin connected to the transistor controlling the fan
#define DHTTYPE DHT11      // DHT11 or DHT22
#define TEMPERATURE_THRESHOLD 25  // Temperature threshold in Celsius

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Blynk Authentication Token
char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_WiFi_SSID";      // Your Wi-Fi SSID
char pass[] = "Your_WiFi_Password";   // Your Wi-Fi password

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature(); // Reading temperature in Celsius
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature!");
    return;
  }

  // Map temperature to PWM fan speed (0 to 255)
  int fanSpeed;
  if (temperature > TEMPERATURE_THRESHOLD) {
    fanSpeed = map(temperature, TEMPERATURE_THRESHOLD, 40, 100, 255); // Map temp to PWM range
    fanSpeed = constrain(fanSpeed, 100, 255); // Minimum speed to avoid stalling
  } else {
    fanSpeed = 0; // Turn off fan if temperature is below threshold
  }

  analogWrite(FAN_PIN, fanSpeed);  // Control fan speed
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(fanSpeed);

  // Send temperature and fan speed data to Blynk
  Blynk.virtualWrite(V1, temperature);   // Send temperature to Virtual Pin V1
  Blynk.virtualWrite(V2, fanSpeed);      // Send fan speed to Virtual Pin V2

  delay(1000); // Update every second
}
