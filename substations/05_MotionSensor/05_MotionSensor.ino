#include <ESP8266WiFi.h>
#include "MotionSensor.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"

#define PIN 13
unsigned int notifCaller = 0;

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
    if (++notifCaller >= 1000) {
        mqtt.send(motion_sensor(PIN));
        notifCaller = 0;
    }
    delay(1);
}
