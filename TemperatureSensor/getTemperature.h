#ifndef GETTEMPERATURE_H
#define GETTEMPERATURE_H

/* Unique values to each Temperature sensor */
#define ID 1
#define TYPE "TEMPERATURE_SENSOR"

/* Function for reading digital or analog temperature */

#include <ArduinoJson.h>
#include <math.h>
#include "ReadDigitalTemperature.h"
#include "ReadAnalogTemperature.h"

String get_temperature(bool digital, int in_pin)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    /* Preparing JSON to send to the broker */
    root["id"] = ID;
    root["type"] = TYPE;
       
    if(digital) {
        root["value"] = read_digital_temperature(in_pin);
    }else {
        root["value"] = read_analog_temperature(in_pin);
    }
    serializeJson(root, json_output); 
    return json_output;
}
#endif /* GETTEMPERATURE_H */
