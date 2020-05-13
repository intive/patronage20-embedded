#include <ESP8266WiFi.h>
#include "MotionSensor.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"

#define PIN 13

MQTT mqtt;
Network network(ssid, passwd);

void setup()
{
    Serial.begin(115200);
    pinMode(PIN, INPUT);
    network.init();
}

void loop()
{
    mqtt.loop();
    mqtt.send(motion_sensor(PIN));
    delay(1000);
}
