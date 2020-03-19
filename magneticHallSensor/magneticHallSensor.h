/* Author: Szymon Wojtach */

#include <ArduinoJson.h>

/* Returns prepared json file filled with sensor status */
String magnetic_digital(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;
    doc["id"] = 77;
    doc["type"] = "windowSensors";

    if (digitalRead(pin) == LOW)
        doc["status"] = "closed";
    else
        doc["status"] = "open";

    serializeJsonPretty(doc, output);
    return output;
}
