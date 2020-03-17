// Author: Szymon Wojtach

#include <Arduino.H>
#include <ESP8266WiFi.h>
#include "MQTT.h"
#include "magneticHallSensor.h"

#define digitalPin 15

const char ssid[] = "ssid_example";
const char passwd[] = "passw_example";
MQTT mqtt;

void setup()
{
  Serial.begin(115200);
  pinMode(digitalPin, INPUT);
  while (!Serial)
  {
    ;
  }
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
}

void loop() 
{
  mqtt.loop();
  mqtt.send(prepJson(digitalPin));
  delay(100);
}
