/*
Author:Patryk Szczodrowski
*/
// #include <ArduinoMqttClient.h>
#include <Arduino.h>
#include "MQTT.h"
#include <ESP8266WiFi.h>        // Include the Wi-Fi library

#include <ArduinoJson.h>

#define LED 2 //Define blinking LED pin

const char ssid[] = "FIND ME";
const char pass[] = "87654321";
DynamicJsonDocument JSONdoc(2048);
// const char serialnumber[] = "12345678";
// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.
MQTT mqtt;
void ledPlain(String s){
  if(s[0]=='0') digitalWrite(LED,HIGH);
  else if(s[0]=='1') digitalWrite(LED,LOW);
}
void ledJSON(String s){
  deserializeJson(JSONdoc, s);
  if(JSONdoc["command"].as<String>().equals("LED"))
    if(JSONdoc["state"].as<int>() == 0)
      digitalWrite(LED, HIGH);
    else if(JSONdoc["state"].as<int>() == 1)
      digitalWrite(LED, LOW);
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(1000);
  }
  // mqtt.setReturnFunct(ledPlain);
  mqtt.setReturnFunct(ledJSON);
  // mqtt.loop();
  // mqtt.send("4321");
}

void loop() {
  mqtt.loop();
}
