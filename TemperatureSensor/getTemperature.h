#include <ArduinoJson.h>
#include <math.h>
#include "ReadDigitalTemperature.h"
#include "ReadAnalogTemperature.h"

String getTemperature(bool digital)
    {
        String JsonOutput = "";
        StaticJsonDocument<200> root;
  
        root["id"] = 1;
        root["type"] = "TemperatureSensor";
        if(digital)
            root["value"] = ReadDigitalTemperature(4);else
            root["value"] = ReadAnalogTemperature(0);

        serializeJson(root, Serial);
        serializeJson(root, JsonOutput);

        return JsonOutput;
    }
