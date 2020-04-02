/* Author: Szymon Wojtach */
#ifndef LINEAR_DIGITAL_H
#define LINEAR_DIGITAL_H

#include <ArduinoJson.h>

/* Returns prepared json file filled with sensor status */
String linear_digital(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;
    doc["id"] = 7;
    doc["type"] = "windowSensors";

    if (digitalRead(pin) == 0)
        doc["status"] = "open";
    else
        doc["status"] = "closed";
    
    serializeJsonPretty(doc, output);
    return output;
}

#endif