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
#define SERVO_1H_OPEN 120 /* Servo for room 1, heating valve etc. */
#define SERVO_1H_CLOSED 30
#define SERVO_1C_OPEN 95
#define SERVO_1C_CLOSED 5
#define SERVO_2H_OPEN 98
#define SERVO_2H_CLOSED 8
#define SERVO_2C_OPEN 99
#define SERVO_2C_CLOSED 9


/* defining pins for Peltier */
#define PELTIER_HEATING 2
#define PELTIER_COOLING 14

/* HVAC Rooms  */
#define NO_OF_ROOMS 2
static const int hvacRooms_R1_ID = 1;
static const int hvacRooms_R2_ID = 2;


static std::vector<int> openWindows;

static struct Room 
{
    int id;
    int hyst = 5;
    int heatingTemp = 50;
    int coolingTemp = 400;
    bool cooling = false;
    bool heating = false; 
    int temSenID = 0;
    std::vector<int> winSenID;
    
    int tempReal;
    int valveHeating_id;
    int valveCooling_id;
    bool valveHeating_isOpen;
    bool valveCooling_isOpen;
    bool termometer_isActive = false;
    
}room[NO_OF_ROOMS];

MQTT mqtt;

static void show_data_SP(Room* room, int i) 
{
    Serial.print("ID: ");
    Serial.println(room[i].id);

    Serial.print("hysteresis: ");
    Serial.println(room[i].hyst);
    
    Serial.print("heatingTemp: ");
    Serial.println(room[i].heatingTemp);

    Serial.print("coolingTemp: ");
    Serial.println(room[i].coolingTemp);

    Serial.print("TempSensorID: ");
    Serial.println(room[i].temSenID);

    Serial.print("heating: ");
    Serial.println(room[i].heating);

    Serial.print("cooling: ");
    Serial.println(room[i].cooling);

    if (room[i].winSenID.size() != 0) {
        Serial.print("WinSensorID: ");
        for(int j = 0; j < room[i].winSenID.size(); j++) {
            Serial.print(room[i].winSenID[j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    
    Serial.print("Real Room Temperature: ");
    Serial.println(room[i].tempReal);
    
    Serial.print("isTermometerActive: ");
    Serial.println(room[i].termometer_isActive);

    Serial.println(" --- ");
    
    if (openWindows.size() != 0) {
        Serial.print("openWindows: ");
        for(int j = 0; j < openWindows.size(); j++) {
            Serial.print(openWindows[j]);
            Serial.print(" ");
        }
            
        Serial.println();
    }
    
    Serial.println(" --- ");
}

static bool idle_exist(std::vector<int> & haystack, int idle) 
{
    bool found = false;
	for (auto & elem : haystack)
	{
		if (elem == idle)
		{
			found = true;
			break;
		}
	}
    return found;   
}

static bool isWindowsOpen(Room* room, int i)
{
    int j;
    for (j = 0; j < room[i].winSenID.size(); j++) {
        if(idle_exist(openWindows, room[i].winSenID[j]))
            return true;
    }
    return false;
}	

static String output_JSON_HVACStatus(bool c, bool h)
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

static String output_JSON_HVACRooms(Room* room, int i)
{
    int j;
    DynamicJsonDocument root(2000);
    String json_output = "";
    root["id"] = room[i].id;
    root["type"] = "HVACRoom";
    root["coolingTemperature"] = room[i].coolingTemp;
    root["heatingTemperature"] = room[i].heatingTemp;
    root["hysteresis"] = room[i].hyst;
    root["cooling"] = room[i].cooling;
    root["heating"] = room[i].heating;
    root["temperatureSensorId"] = room[i].temSenID;
    for (j = 0; j < room[i].winSenID.size(); j++) 
        root["windowSensorIds"].add(room[i].winSenID[j]);
    
    serializeJson(root, json_output);
    return json_output;
}

static int ledPIN_fromServoID(int id) 
{
    switch(id) {
        case SERVO_1H_ID:  return SERVO_1H_LED;
        case SERVO_1C_ID:  return SERVO_1C_LED;
        case SERVO_2H_ID:  return SERVO_2H_LED;
        case SERVO_2C_ID:  return SERVO_2C_LED;
    }
}

static int angle_fromServoID(int id, bool isOpen) 
{
    if(isOpen) {
        switch(id) {
            case SERVO_1H_ID:  return SERVO_1H_OPEN;
            case SERVO_1C_ID:  return SERVO_1C_OPEN;
            case SERVO_2H_ID:  return SERVO_2H_OPEN;
            case SERVO_2C_ID:  return SERVO_2C_OPEN;
        }
    } 
    else {
        switch(id) {
            case SERVO_1H_ID:  return SERVO_1H_CLOSED;
            case SERVO_1C_ID:  return SERVO_1C_CLOSED;
            case SERVO_2H_ID:  return SERVO_2H_CLOSED;
            case SERVO_2C_ID:  return SERVO_2C_CLOSED;
        }
    }
}

static void set_valve(int id, bool position) 
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = "Servo";
    root["angle"] = angle_fromServoID(id, position);
    
    serializeJson(root, json_output);
    mqtt.send(mqttInputTopic, json_output);
    
}

static void updateRooms(Room* room, DynamicJsonDocument& json_doc) 
{
    int i;
    int j;

    for (i = 0; i < NO_OF_ROOMS; i++) {
        if(json_doc["id"].as<int>() == (i + 1)) {
            if (json_doc.containsKey("hysteresis"))
                room[i].hyst = json_doc["hysteresis"];
            if (json_doc.containsKey("heatingTemperature"))
                room[i].heatingTemp = json_doc["heatingTemperature"];
            if (json_doc.containsKey("coolingTemperature"))
                room[i].coolingTemp = json_doc["coolingTemperature"];
            if (json_doc.containsKey("temperatureSensorId")) {
                room[i].temSenID = json_doc["temperatureSensorId"];
                room[i].termometer_isActive = false;
            }
            if (json_doc.containsKey("windowSensorIds")) {
                room[i].winSenID.clear();
                for(j = 0; j < json_doc["windowSensorIds"].size(); j++) {
                    room[i].winSenID.push_back(json_doc["windowSensorIds"][j]);
                    /* when new windowIds is added - assume is open
                        - put in openWindows container, if not already there before */
                    if(!idle_exist(openWindows, json_doc["windowSensorIds"][j])){
                        openWindows.push_back(json_doc["windowSensorIds"][j]);
                    }
                }
            }
        }
    }
}

static void updateTermometer(Room* room, DynamicJsonDocument& json_doc) 
{
    int i;

    for (i = 0; i < NO_OF_ROOMS; i++) {
        if(json_doc["id"].as<int>() == room[i].temSenID) {
            room[i].tempReal = json_doc["value"];
            room[i].termometer_isActive = true;
            Serial.println("Updated temp.");
        }
    }
}

static void updateValve(Room* room, DynamicJsonDocument& json_doc) 
{
    int i;
    
    for (i = 0; i < NO_OF_ROOMS; i++) {
        /* check if servo is responsible for heating valve */
        if(json_doc["id"].as<int>() == room[i].valveHeating_id) {
            if (json_doc["angle"] == angle_fromServoID(json_doc["id"].as<int>(), true)) {
                room[i].valveHeating_isOpen = true;
            }
            else if (json_doc["angle"] == angle_fromServoID(json_doc["id"].as<int>(), false)) {
                room[i].valveHeating_isOpen = false;
            }
        /* set LED indicator for heating Valve (servo) */
        digitalWrite(ledPIN_fromServoID(room[i].valveHeating_id), room[i].valveHeating_isOpen);    
        }
        /* check if servo is responsible for cooling valve */
        else if(json_doc["id"].as<int>() == room[i].valveCooling_id) {
            if (json_doc["angle"] == angle_fromServoID(json_doc["id"].as<int>(), true)){
                room[i].valveCooling_isOpen = true;
            }
            else if (json_doc["angle"] == angle_fromServoID(json_doc["id"].as<int>(), false)){
                room[i].valveCooling_isOpen = false;  
            }
        /* set LED indicator for cooling Valve (servo) */
        digitalWrite(ledPIN_fromServoID(room[i].valveCooling_id), room[i].valveCooling_isOpen);
        }
    }
}

static void updateWindowsSens(DynamicJsonDocument& json_doc) 
{
    if(json_doc["status"].as<String>().equals("open")) {
        /* put window ID into container with open windows (if not exist) */
        if(!idle_exist(openWindows, json_doc["id"].as<int>())){
            openWindows.push_back(json_doc["id"].as<int>());
        }
    }
    else if(json_doc["status"].as<String>().equals("closed")) {
        /* erase closed window ID from container wit oped windows */
        openWindows.erase(std::remove(openWindows.begin(), openWindows.end(), json_doc["id"].as<int>()), openWindows.end());
    }
}

enum pltrStatus {COOLING = 0, HEATING = 1};

/* Peltier overheating or overcooling protection */
static bool peltierProtect(Room* room, int status)
{
    int i;
    bool protect = true;

    for(i = 0; i < NO_OF_ROOMS; i++) {
        if (status == 0 && room[i].valveCooling_isOpen == true)
            protect = false;
        else if (status == 1 && room[i].valveHeating_isOpen == true)
            protect = false;
    }
    
    return protect;
}
#endif HVAC_H