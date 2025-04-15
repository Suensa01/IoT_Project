#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11
#define SOIL_PIN A0
#define RAIN_PIN D1
#define RELAY_PIN D5

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";
const String serverUrl = "http://your_computer_ip:5000/api/farm";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int soil = analogRead(SOIL_PIN);
    bool raining = digitalRead(RAIN_PIN) == LOW;
    
    // Control irrigation
    if (soil < 500 && !raining) {
      digitalWrite(RELAY_PIN, LOW);
    } else {
      digitalWrite(RELAY_PIN, HIGH);
    }

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"soil_moisture\":" + String(soil) + 
                  ",\"temperature\":" + String(temp) + 
                  ",\"humidity\":" + String(hum) + 
                  ",\"rain\":\"" + (raining ? "yes" : "no") + "\"}";

    int httpCode = http.POST(json);
    Serial.println(httpCode);
    Serial.println(http.getString());

    http.end();
  }

  delay(10000);
}
