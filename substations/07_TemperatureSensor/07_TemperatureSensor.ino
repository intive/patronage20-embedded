/* Temperature sensor */

#include "getTemperature.h"
#include "esp_pins.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"

/* Network connecion library - ssid and password got from init_config.h */
Network network(ssid, passwd);

/* Communication library */
MQTT mqtt;

int digital_pin = D2;
int analog_pin = A0;

/* Function to reporting temperature to the broker */
void raport_temperature(String json_output)
{
    /* Send JSON object over the MQTT */
    mqtt.send(json_output);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }

    pinMode(digital_pin, INPUT);
    pinMode(D8, INPUT);

    /* Initializing Wifi network */
    network.init();
}

void loop()
{
    /* Keep network alive */
    network.loop();
    /* Keep MQTT alive */
    mqtt.loop();
    /* Report temperature from digital sensor to the broker */
    raport_temperature(get_temperature(true, digital_pin));
    /* Report temperature from analog sensor to the broker */
    raport_temperature(get_temperature(false, analog_pin));

    delay(1000);
}
