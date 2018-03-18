#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "Wifi.h" 

WiFiClient myclient;

void setup() {
    Serial.begin(9600);

    // https://arduinojson.org/assistant/ - That will help you
    const size_t capacity = 4*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4);
    DynamicJsonBuffer jsonBuffer(capacity);
    
    String payload;
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    HTTPClient http;
    http.begin("http://query.yahooapis.com/v1/public/yql?q=select%20item.forecast.high,%20item.forecast.low,%20item.condition.temp,%20item.forecast.text%20from%20weather.forecast%20where%20woeid%20=590999%20and%20u=%27c%27%20limit%201&format=json");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode <= 0) return;
    
    payload = http.getString();
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }

    long t = root["query"]["results"]["channel"]["item"]["condition"]["temp"];
    Serial.println(t);
    String x = root["query"]["results"]["channel"]["item"]["forecast"]["text"];
    Serial.println(x);
}

void loop() { }
