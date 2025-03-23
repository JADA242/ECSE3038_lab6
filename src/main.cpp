#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <OneWire.h>
#include "env.h"


OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();

  if (IS_WOKWI) {
    WiFi.begin(SSID, PASS, CHANNEL);
  } else {
    WiFi.begin(SSID, PASS);
  }

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi:");
  Serial.println(WiFi.localIP());

  pinMode(LED_PIN, OUTPUT);
}


void getLight() {
  HTTPClient http;
  http.begin(String(ENDPOINT) + "/api/light");
  http.addHeader("api-key", API_KEY);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    String responseBody = http.getString();
    Serial.println("Response: " + responseBody);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, responseBody);

    if (error) {
      Serial.println("JSON deserialization failed");
      http.end();
      return;
    }

    bool light_status = doc["light"];
    digitalWrite(LED_PIN, light_status);
    Serial.println(light_status ? "LED ON" : "LED OFF");
  } else {
    Serial.print("Error in GET request. Code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}


void putTemperature() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  
  Serial.print("temp: ");
  Serial.println(temperature);

  HTTPClient http;
  http.begin((String) ENDPOINT + "/api/temp");
  http.addHeader("api-key", API_KEY);
  http.addHeader("Content-Type", "application/json");

  JsonDocument doc;
  doc["temp"] = temperature;
  doc.shrinkToFit();

  String httpRequestBody;
  serializeJson(doc, httpRequestBody);



  int httpResponseCode = http.PUT(httpRequestBody);

  Serial.println("Response Code: " + String(httpResponseCode));
  Serial.println(http.getString());
  http.end();
}


void loop() {
  getLight();       
  putTemperature();  
  delay(3000);       
}
