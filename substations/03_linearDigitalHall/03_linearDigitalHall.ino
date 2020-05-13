/* Author: Szymon Wojtach */

#include "linearDigitalHall.h"
#include "MQTT.h"
#include "Network.h"

#define digitalPin 15

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
    network.loop();
    mqtt.loop();
    mqtt.send(linear_digital(digitalPin));
    delay(1);
}
