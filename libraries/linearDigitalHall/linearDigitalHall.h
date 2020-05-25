/* Author: Szymon Wojtach */
#ifndef LINEAR_DIGITAL_H
#define LINEAR_DIGITAL_H

#include <ArduinoJson.h>
#include "init_config.h"

/* Returns prepared json file filled with sensor status */
String linear_digital(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;
    doc["id"] = windowSensors_linear_ID;
    doc["type"] = "windowSensor";

    if (digitalRead(pin) == 0)
        doc["status"] = "open";
    else
        doc["status"] = "closed";
    
    serializeJson(doc, output);
    return output;
}

#endif