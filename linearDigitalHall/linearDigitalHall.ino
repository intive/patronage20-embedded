// Author: Szymon Wojtach

#include <Arduino.H>
#include <ESP8266WiFi.h>
#include "MQTT.h"
#include "linearDigitalHall.h"

#define digitalPin 15

const char ssid[] = "example_ssid";
const char passwd[] = "example_password";
MQTT mqtt;

void setup()
{
  Serial.begin(115200);
  pinMode(digitalPin, INPUT);
  while (!Serial) {
    ;
  }
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
}

void loop()
{
  mqtt.loop();
  mqtt.send(linearDigital(digitalPin));
  delay(100);
}
