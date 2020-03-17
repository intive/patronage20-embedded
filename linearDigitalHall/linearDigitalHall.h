// Author: Szymon Wojtach

#include <ArduinoJson.h>

enum window_sensor_state{
    open,
    closed
};

int value;

// Returns prepared json file filled with sensor status
String prepJson(int pin)
{
    String output = "";
    StaticJsonDocument<100> doc;

    doc["id"] = 7;
    doc["type"] = "windowSensors";
    if(digitalRead(pin) == window_sensor_state::open)
    {
        doc["value"] = "open";
    }
    else
    {
        doc["value"] = "closed";
    }
    
    serializeJsonPretty(doc, output);
    return output;
}
