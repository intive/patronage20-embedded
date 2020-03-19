#include <ESP8266WiFi.h>
#include "MotionSensor.h"
#include "MQTT.h"

#define PIN 13

const char ssid[] = "ssid";
const char passwd[] = "passwd";

MQTT mqtt; 

void setup()
{
    Serial.begin(115200);
    pinMode(PIN, INPUT);
    while (!Serial) {
        ;
    }
    Serial.print("Trying to connect to: ");
    Serial.println(ssid);
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
}

void loop() 
{
    mqtt.loop();
    mqtt.send(motion_sensor(PIN));
    delay(100);
}
