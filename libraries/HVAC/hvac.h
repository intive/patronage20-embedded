#ifndef HVAC_H
#define HVAC_H


#include <ArduinoJson.h>
#include "init_config.h"
#include "MQTT.h"

/* defining pins for LED that indicates Servo status */
#define SERVO_1H_LED 16 /* Servo for room 1, heating valve etc. */
#define SERVO_1C_LED 5
#define SERVO_2H_LED 4
#define SERVO_2C_LED 0

/* defining ID's for Servos */
#define SERVO_1H_ID 101 /* Servo for room 1, heating valve etc. */
#define SERVO_1C_ID 102
#define SERVO_2H_ID 103
#define SERVO_2C_ID 104

/* defining angle for valve fully open and closed */
#define VALVE_OPEN 90
#define VALVE_CLOSED 0

/* defining pins for Peltier */
#define PELTIER_HEATING 2
#define PELTIER_COOLING 14

MQTT mqtt;

String output_JSON_HVACStatus(bool c, bool h)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = HVACStatus_ID;
    root["type"] = "HVACStatus";
    root["cooling"] = c;
    root["heating"] = h;
    serializeJson(root, json_output);
    return json_output;
}

String output_JSON_HVACRooms(int id, int hyst, int heatTemp, int coolingTemp, bool cooling, bool heating, int temSenID,  int *winSeId)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = "HVACRooms";
    root["coolingTemperature"] = heatTemp;
    root["heatingTemperature"] = coolingTemp;
    root["cooling"] = cooling;
    root["heating"] = heating;
    
    serializeJson(root, json_output);
    return json_output;
}

int ledPIN_fromServoID(int id) 
{
    switch(id) {
        case SERVO_1H_ID:  return SERVO_1H_LED;
        case SERVO_1C_ID:  return SERVO_1C_LED;
        case SERVO_2H_ID:  return SERVO_2H_LED;
        case SERVO_2C_ID:  return SERVO_2C_LED;
    }
}

void set_valve(int id, bool position) 
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = "Servo";

    if (position) {
        root["angle"] = VALVE_OPEN;
        digitalWrite(ledPIN_fromServoID(id), HIGH);
    } else {
        root["angle"] = VALVE_CLOSED;
        digitalWrite(ledPIN_fromServoID(id), LOW);
    }
    
    serializeJson(root, json_output);
    mqtt.send(mqttInputTopic, json_output);
    
}

enum peltier {COOLING = 0, HEATING = 1};

void set_peltier(int type, bool mode) 
{
    if (type == 0) {
        digitalWrite(PELTIER_COOLING, mode);
    }
    if (type == 1) {
        digitalWrite(PELTIER_HEATING, mode);
    }
}

#endif HVAC_H