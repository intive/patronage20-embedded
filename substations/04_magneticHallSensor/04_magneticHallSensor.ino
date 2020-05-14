/* Author: Szymon Wojtach */

#include "magneticHallSensor.h"
#include "MQTT.h"
#include "Network.h"

#define digitalPin 15
unsigned int notif_counter = 0;

MQTT mqtt;
Network network(ssid, passwd);

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }
    pinMode(digitalPin, INPUT);
    network.init();
}

void loop()
{
    mqtt.loop();
    if (++notif_counter >= 1000) {
        mqtt.send(magnetic_digital(digitalPin));
        notif_counter = 0;
    }
    delay(1);
}
