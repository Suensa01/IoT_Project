#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

DHT dht(DHTPIN, DHTTYPE);
int heartRatePin = A0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    float temperature = dht.readTemperature();
    int heartRate = analogRead(heartRatePin);  // Simulate reading heart rate

    http.begin("http://<YOUR_FLASK_IP>:5000/api/health");  // Replace with your IP
    http.addHeader("Content-Type", "application/json");

    String json = "{\"heart_rate\":" + String(heartRate) + ",\"temperature\":" + String(temperature) + "}";
    int httpCode = http.POST(json);
    String payload = http.getString();

    Serial.println(httpCode);
    Serial.println(payload);

    http.end();
  }
  delay(10000); // Post data every 10 seconds
}
