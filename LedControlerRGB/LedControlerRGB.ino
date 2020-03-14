#include <ArduinoJson.h>
#include "HSVtoRGB.h"

bool SetLED_RGB_Value(int redPIN, int greenPIN, int bluePIN, String JsonInput)
    {
        int RGB[3];
        int hue, saturation, value;  
        StaticJsonDocument<200> doc;

        DeserializationError error = deserializeJson(doc, JsonInput);

        if (error)
            return false;
            
        hue = doc["hue"];
        saturation = doc["saturation"];
        value = doc["value"];
        
        HSVtoRGB(hue, double (saturation)/double(100), double (value)/double(100), RGB);
        
        analogWrite(redPIN,RGB[0]);        //set PWM value for red
        analogWrite(greenPIN, RGB[1]);     //set PWM value for blue
        analogWrite(bluePIN,RGB[2]);       //set PWM value for green

        return true;
    }
