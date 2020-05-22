#include <Servo.h>
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include <ArduinoJson.h>

#define PIN 15
int id = Servo_1_ID;
String type = "Servo";

MQTT mqtt;
Network network(ssid, passwd);
Servo servo;
int servoAng;
int notifCaller = 0;

String output_JSON(int a, int id, String t)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = t;
    root["angle"] = a;
    serializeJson(root, json_output);
    return json_output;
}

void incoming_JSON(String json_input)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    if (error) {
        return;
    }
    if(json_doc["id"].as<int>() == id)
        if(json_doc["type"].as<String>().equals(type)) {
            servoAng = json_doc["angle"].as<int>();
            servo.write(servoAng);
        }
}

void setup() 
{
    servo.attach(PIN);
    servo.write(10);
    Serial.begin(9600);
    network.init();
    mqtt.set_return_function(incoming_JSON);
}

void loop() 
{ 
    mqtt.loop();

    if (++notifCaller == 1000) {
        mqtt.send(output_JSON(servoAng, id, type));
        notifCaller = 0;
    }
    
    delay(1);
    
} 
