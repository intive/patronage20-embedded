#ifndef GETTEMPERATURE_H
#define GETTEMPERATURE_H

/* Function for reading digital or analog temperature */

#include <ArduinoJson.h>
#include <math.h>
#include "ReadDigitalTemperature.h"
#include "ReadAnalogTemperature.h"
#include "init_config.h"

String get_temperature(bool digital, int in_pin)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    /* Preparing JSON to send to the broker */

    /* Unique values to each Temperature sensor ID and TYPE included in init_config.h */
    root["id"] = id_temp_sensor_1;
    root["type"] = type_temp_sensor;

    if(digital) {
        root["value"] = read_digital_temperature(in_pin);
    }else {
        root["value"] = read_analog_temperature(in_pin);
    }
    serializeJson(root, json_output);
    return json_output;
}
#endif /* GETTEMPERATURE_H */
