#include <ArduinoJson.h>
#include <math.h>
#include "ReadDigitalTemperature.h"
#include "ReadAnalogTemperature.h"

String getTemperature(bool digital, int inPin)
    {
        String JsonOutput = "";
        StaticJsonDocument<200> root;
  
        root["id"] = 1;
        root["type"] = "TemperatureSensor";
        if(digital)
            root["value"] = ReadDigitalTemperature(inPin);else
            root["value"] = ReadAnalogTemperature(inPin);

        serializeJson(root, JsonOutput);

        return JsonOutput;
    }
