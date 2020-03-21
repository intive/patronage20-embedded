/* Demonstrating set_led_rgb_value and hsv_to_rgb */

#include <ArduinoJson.h>
#include "HSVtoRGB.h"

String json_output = "";
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

bool set_led_rgb_value(int red_pin, int green_pin, int blue_pin, String json_input)
{
    int RGB[3];
    int hue, saturation, value;  
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, json_input);

    if (error)
        return false;
            
    hue = doc["hue"];
    saturation = doc["saturation"];
    value = doc["value"];
        
    hsv_to_rgb(hue, saturation, value, RGB);
        
    analogWrite(red_pin,RGB[0]);        /* set PWM value for red */
    analogWrite(green_pin, RGB[1]);     /* set PWM value for blue */
    analogWrite(blue_pin, RGB[2]);      /* set PWM value for green */

    return true;
}

void setup() 
{
    Serial.begin(9600);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
}

void loop() 
{
    StaticJsonDocument<200> root;
    json_output = "";
    root["hue"] = random(0,360);
    root["saturation"] = random(0,100);
    root["value"] = random(0,100);

    serializeJson(root, json_output);
    set_led_rgb_value(D6, D5, D7, json_output);
    delay(100);  
}
