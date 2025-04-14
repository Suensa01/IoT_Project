#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

const char* server = "http://your-server-ip:5000/api/traffic"; // Flask IP

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    int vehicleCount = random(10, 80); // Replace with actual sensor
    float speed = random(30, 80);

    String data = "{\"location\":\"Kalpana Square\",\"vehicle_count\":" + String(vehicleCount) + ",\"speed\":" + String(speed) + "}";
    
    int httpCode = http.POST(data);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);

    http.end();
  }
  delay(10000); // 10 seconds
}
