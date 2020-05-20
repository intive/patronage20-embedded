/* Author: Szymon Wojtach */
#ifndef MAGNETIC_DIGITAL_H
#define MAGNETIC_DIGITAL_H

#include <ArduinoJson.h>
#include "init_config.h"

/* Returns prepared json file filled with sensor status */
String magnetic_digital(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;
    doc["id"] = windowSensors_magnetic_ID;
    doc["type"] = "windowSensor";

    if (digitalRead(pin) == HIGH)
        doc["status"] = "open";
    else
        doc["status"] = "closed";

    serializeJson(doc, output);
    return output;
}

#endif