// Author: Szymon Wojtach

#include <ArduinoJson.h>

// Returns prepared json file filled with sensor status
String prepJson(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;

    doc["id"] = 7;
    doc["type"] = "windowSensors";
    if(digitalRead(pin) == LOW)
    {
        doc["value"] = "closed";
    }
    else
    {
        doc["value"] = "open";
    }
    
    serializeJsonPretty(doc, output);
    return output;
}
