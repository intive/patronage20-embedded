/* Temperature sensor */

#include "getTemperature.h"
#include "esp_pins.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"

/* ssid and password got from init_config.h */
Network network(ssid, passwd);

MQTT mqtt;

int digital_pin = D2;
int analog_pin = A0;

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }

    pinMode(digital_pin, INPUT);
    pinMode(D8, INPUT);

    network.init();
}

void loop()
{
    network.loop();
    mqtt.loop();
    /* Report temperature from digital sensor to the broker */
    mqtt.send(get_temperature(true, digital_pin));
    /* Report temperature from analog sensor to the broker */
    mqtt.send(get_temperature(false, analog_pin));

    delay(1000);
}
