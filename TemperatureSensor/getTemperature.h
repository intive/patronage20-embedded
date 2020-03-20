/* Function for reading digital or analog temperature */

#include <ArduinoJson.h>
#include <math.h>
#include "ReadDigitalTemperature.h"
#include "ReadAnalogTemperature.h"

String get_temperature(bool digital, int in_pin)
{
    String JsonOutput = "";
    StaticJsonDocument<200> root;
  
    root["id"] = 1;
    root["type"] = "TemperatureSensor";
        
    if(digital) {
        root["value"] = read_digital_temperature(in_pin);
    }else {
        root["value"] = read_analog_temperature(in_pin);
    }
    serializeJson(root, JsonOutput);
    return JsonOutput;
}
