#include <ESP8266WiFi.h>
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
   
    if (myclient.connect("query.yahooapis.com", 80)) {
        Serial.println(F("Connected to server.."));
        
        myclient.println(F("GET /v1/public/yql?q=select%20item.forecast.high,item.forecast.low,item.forecast.text,item.condition.temp%20from%20weather.forecast%20where%20woeid=590999%20and%20u='c'%20limit%201&format=json HTTP/1.0")); //toulon
        myclient.println(F("HOST:query.yahooapis.com"));
        myclient.println(F("Connection: close"));
        myclient.println();
    }
        
    char status[32] = {0};
    myclient.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }
  
    char endOfHeaders[] = "\r\n\r\n";
    if (!myclient.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }
    
    JsonObject& root = jsonBuffer.parseObject(myclient);
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
