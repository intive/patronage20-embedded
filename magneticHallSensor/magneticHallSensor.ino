/* Author: Szymon Wojtach */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MQTT.h"
#include "magneticHallSensor.h"

#define digitalPin 15

const char ssid[] = "ssid_example";
const char passwd[] = "password_example";
MQTT mqtt;

void setup()
{
    Serial.begin(115200);
    pinMode(digitalPin, INPUT);
    while (!Serial) {
        delay(100);
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
    mqtt.send(magnetic_digital(digitalPin));
    delay(100);
}
