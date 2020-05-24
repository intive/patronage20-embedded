#include <Servo.h>
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include <ArduinoJson.h>

/* defining PIN's for Servo */
#define SERVO_1H_PIN 16 /* D0 */
#define SERVO_1C_PIN 5  /* D1 */
#define SERVO_2H_PIN 4  /* D2 */
#define SERVO_2C_PIN 0  /* D3 */

/* defining ID's for Servos */
const int id_1H = Servo_1H_ID; /* 101 */
const int id_1C = Servo_1C_ID; /* 102 */
const int id_2H = Servo_2H_ID; /* 103 */
const int id_2C = Servo_2C_ID; /* 104 */

/* variables storing angles for servo's */
int ang_1H;
int ang_1C;
int ang_2H;
int ang_2C;

String type = "Servo";

MQTT mqtt;
Network network(ssid, passwd);

Servo servo_1H;
Servo servo_1C;
Servo servo_2H;
Servo servo_2C;

int notifCaller = 0;

int getPin_fromServoID(int id) 
{
    switch(id) {
        case id_1H:  return SERVO_1H_PIN;
        case id_1C:  return SERVO_1C_PIN;
        case id_2H:  return SERVO_2H_PIN;
        case id_2C:  return SERVO_2C_PIN;
    }
}

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
    int id;
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    
    if (error) {
        return;
    }
    
    id = json_doc["id"].as<int>();

    
    if(json_doc["type"].as<String>().equals(type) 
       && json_doc["angle"] >= 0 
       && json_doc["angle"] <=180) {
        
        switch(id) {
            case id_1H: ang_1H = json_doc["angle"]; servo_1H.write(ang_1H); break;
            case id_1C: ang_1C = json_doc["angle"]; servo_1C.write(ang_1C);break;
            case id_2H: ang_2H = json_doc["angle"]; servo_2H.write(ang_2H);break;
            case id_2C: ang_2C = json_doc["angle"]; servo_2C.write(ang_2C);break;
        }
    }
}

void setup()
{
    servo_1H.attach(getPin_fromServoID(id_1H));
    servo_1C.attach(getPin_fromServoID(id_1C));
    servo_2H.attach(getPin_fromServoID(id_2H));
    servo_2C.attach(getPin_fromServoID(id_2C));
    
    servo_1H.write(10);
    servo_1C.write(10);
    servo_2H.write(10);
    servo_2C.write(10);
    
    Serial.begin(9600);
    network.init();
    mqtt.set_return_function(incoming_JSON);
}

void loop()
{
    mqtt.loop();

    if (++notifCaller == 1000) {
        mqtt.send(output_JSON(ang_1H, id_1H, type));
        mqtt.send(output_JSON(ang_1C, id_1C, type));
        mqtt.send(output_JSON(ang_2H, id_2H, type));
        mqtt.send(output_JSON(ang_2C, id_2C, type));
        notifCaller = 0;
    }

    delay(1);

}
