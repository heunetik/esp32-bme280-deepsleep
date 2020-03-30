#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 5
#define WLAN_SSID ""
#define WLAN_PASS ""

const int timeout = 200;

Adafruit_BME280 bme;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(800);

  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor!");
    goToDeepSleep();
  }

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  int i = 0;
  for (; i < timeout; i++) {
    if (WiFi.status() == WL_CONNECTED) break;
    delay(100);
  }
  if (i == timeout) {
    goToDeepSleep();
  }

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  sendSensorData();
  goToDeepSleep();
}

void loop() {}

void sendSensorData() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin("");
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST("{\"sensor\": \"bme280\", \"data\": [" +
      String(bme.readTemperature()) + ", " +
      String(bme.readHumidity()) + ", " +
      String(bme.readPressure() / 100.0F) + "]}");
    if (httpResponseCode < 0) {
      Serial.print("HTTP client error: ");
    }
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi connection error!");
  }
}

void goToDeepSleep() {
  Serial.println("Turning off WiFi");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
}