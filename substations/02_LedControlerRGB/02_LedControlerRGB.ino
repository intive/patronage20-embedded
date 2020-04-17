/* LED controller */

#include <ArduinoJson.h>
#include "HSVtoRGB.h"
#include "esp_pins.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"

/* Unique values to each LED controller */
#define ID 1
#define TYPE "LED_CONTROLLER"

/* Varibles to store a color */
int hue = 0, saturation = 0, value = 0;

/* Communication library */
MQTT mqtt;
/* Network connecion library - ssid and password got from init_config.h */
Network network(ssid, passwd);

/* Callback function for MQTT library */
void get_hsv_color(String json_input)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    /* If parsing json_doc throws error, return get_hsv_color function */
    if (error)
        return;
    /* Checking if the message concerns our controller */
    if(json_doc["id"].as<int>() == ID)
        if(json_doc["type"].as<String>().equals(TYPE))
        {
            /* Store recivied color */
            hue = json_doc["hue"].as<int>();
            saturation = json_doc["saturation"].as<int>();
            value = json_doc["value"].as<int>();
            /* Setting LED the color received by MQTT */
            set_led_rgb_value(D6, D5, D7);
        }
}

/* Function to control led based on the received from MQTT hue, saturation and value */
void set_led_rgb_value(int red_pin, int green_pin, int blue_pin)
{
    int rgb[3];

    /* Convert HSV to RGB color */
    hsv_to_rgb(hue, saturation, value, rgb);

    analogWrite(red_pin,rgb[0]);        /* set PWM value for red */
    analogWrite(green_pin, rgb[1]);     /* set PWM value for blue */
    analogWrite(blue_pin, rgb[2]);      /* set PWM value for green */

}

void setup()
{
    Serial.begin(9600);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    /* Initializing Wifi network */
    network.init();
    /* Set callback function geting HSV color for MQTT */
    mqtt.set_return_function(get_hsv_color);
}

void loop()
{
    /* Keep network alive */
    network.loop();
    /* Keep MQTT alive */
    mqtt.loop();

    delay(1000);
}
