#include <ESP8266WiFi.h>
#include "MotionSensor.h"
#include "MQTT.h"
#include "init_config.h"

#define PIN 13

void setup()
{
    Serial.begin(115200);
    pinMode(PIN, INPUT);
}

void loop()
{
    Serial.println(motion_sensor(PIN));
    delay(100);
}
