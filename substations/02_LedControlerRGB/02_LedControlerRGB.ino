/* LED controller */
#include <ArduinoJson.h>
#include "HSVtoRGB.h"
#include "esp_pins.h"
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include "report_hsv_color.h"

/* Varibles to store a color */
int hue = 0, saturation = 0, value = 0;

const int ID_LED = 62;
const String TYPE_LED = "LED_CONTROLLER";

MQTT mqtt;
/* ssid and password got from init_config.h */
Network network(ssid, passwd);

unsigned int notifCaller = 0;

/* Callback function for MQTT library */
void get_hsv_color(String json_input)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    if (error)
        return;
    /* ID_LED, TYPE_LED from init_config.h */
    if(json_doc["id"].as<int>() == ID_LED)
        if(json_doc["type"].as<String>().equals(TYPE_LED))
        {
            hue = json_doc["hue"].as<int>();
            saturation = json_doc["saturation"].as<int>();
            value = json_doc["value"].as<int>();
            set_led_rgb_value(D6, D5, D7);
        }
}

void set_led_rgb_value(int red_pin, int green_pin, int blue_pin)
{
    int rgb[3];
    hsv_to_rgb(&hue, &saturation, &value, rgb);
    analogWrite(red_pin,rgb[0]);
    analogWrite(green_pin, rgb[1]);
    analogWrite(blue_pin, rgb[2]);
}

void setup()
{
    Serial.begin(9600);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    network.init();
    mqtt.set_return_function(get_hsv_color);
}

void loop()
{
    mqtt.loop();
    if (++notifCaller >= 1000) {
        mqtt.send(report_hsv_color(hue, saturation, value, ID_LED, TYPE_LED));
        notifCaller = 0;
    }
    delay(1);
}
