/* Author: Szymon Wojtach */
#ifndef MAGNETIC_DIGITAL_H
#define MAGNETIC_DIGITAL_H

#include <ArduinoJson.h>

/* Returns prepared json file filled with sensor status */
String magnetic_digital(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;
    doc["id"] = 77;
    doc["type"] = "windowSensors";

    if (digitalRead(pin) == HIGH)
        doc["status"] = "open";
    else
        doc["status"] = "closed";

    serializeJsonPretty(doc, output);
    return output;
}

#endif