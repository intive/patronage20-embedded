/*
 * WARNING! At "init_config.h" - set the value:
 * #define EMBEDDED_OUTPUT_SUBSCR 1
 * to provide topic "embedded_input" subscription, that is required for HVACRooms.
 */

#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include "hvac.h"
#include <ArduinoJson.h>

#define HVAC_DEBUG 1

unsigned int notifCaller = 0;
Network network(ssid, passwd);


void incoming_JSON(String json_input)
{
    int i;
    int j;
    int k;
    
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    if (error) {
        return;
    }

    /* Gathering Room info from request */
    if(json_doc["type"].as<String>().equals("HVACRoom")) {
        for (i = 0; i < NO_OF_ROOMS; i++) {
            if(json_doc["id"].as<int>() == (i + 1)) {
                if (json_doc.containsKey("hysteresis"))
                    room[i].hyst = json_doc["hysteresis"].as<int>();
                if (json_doc.containsKey("heatingTemperature"))
                    room[i].heatingTemp = json_doc["heatingTemperature"].as<int>();
                if (json_doc.containsKey("coolingTemperature"))
                    room[i].coolingTemp = json_doc["coolingTemperature"].as<int>();
                if (json_doc.containsKey("temperatureSensorId")) {
                    room[i].temSenID = json_doc["temperatureSensorId"].as<int>();
                    room[i].isTermometerActive = false;
                }
                    
                if (json_doc.containsKey("windowSensorIds")) {
                    room[i].winSenID.clear();
                    for(j = 0; j < json_doc["windowSensorIds"].size(); j++) 
                        room[i].winSenID.push_back(json_doc["windowSensorIds"][j]);
                }
            }
        }
        return;
    }
    
    /* Gathering information from termometer */
    if(json_doc["type"].as<String>().equals("TEMPERATURE_SENSOR")) {
        for (i = 0; i < NO_OF_ROOMS; i++) {
            if(json_doc["id"].as<int>() == room[i].temSenID) {
                room[i].tempReal = json_doc["value"].as<int>();
                room[i].isTermometerActive = true;
            }
        }
        return;
    }

    /* Gathering information from Servo */
    if(json_doc["type"].as<String>().equals("Servo")) {
        for (i = 0; i < NO_OF_ROOMS; i++) {
            /* check if servo is responsible for heating valve */
            if(json_doc["id"].as<int>() == room[i].ValveHeating_id) {
                if (json_doc["angle"].as<int>() == angle_fromServoID(json_doc["id"].as<int>(), true)) {
                    room[i].ValveHeating_isOpen = true;
                }
                else if (json_doc["angle"].as<int>() == angle_fromServoID(json_doc["id"].as<int>(), false)) {
                    room[i].ValveHeating_isOpen = false;
                }  
            }
            /* check if servo is responsible for cooling valve */
            else if(json_doc["id"].as<int>() == room[i].ValveCooling_id) {
                if (json_doc["angle"].as<int>() == angle_fromServoID(json_doc["id"].as<int>(), true)){
                    room[i].ValveCooling_isOpen = true;
                }
                else if (json_doc["angle"].as<int>() == angle_fromServoID(json_doc["id"].as<int>(), false)){
                    room[i].ValveCooling_isOpen = false;  
                }
            }
        }
        return;
    }

    /* gathering information about windowSensor status */
    if(json_doc["type"].as<String>().equals("windowSensor")) {
        if(json_doc["status"].as<String>().equals("open")) {
            if(!idle_exist(openWindows, json_doc["id"].as<int>())){
                openWindows.push_back(json_doc["id"].as<int>());
            }
        }
        if(json_doc["status"].as<String>().equals("closed")) {
            openWindows.erase(std::remove(openWindows.begin(), openWindows.end(), json_doc["id"].as<int>()), openWindows.end());
        }
    }
}

void setup() {

    /* set innitial values */
    room[0].id = hvacRooms_R1_ID;
    room[0].ValveHeating_id = SERVO_1H_ID;
    room[0].ValveCooling_id = SERVO_1C_ID;
    room[1].id = hvacRooms_R2_ID;
    room[1].ValveHeating_id = SERVO_2H_ID;
    room[1].ValveCooling_id = SERVO_2C_ID;
    
    pinMode(16, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(14, OUTPUT);

    Serial.begin(9600);
    network.init();
    mqtt.set_return_function(incoming_JSON);
}

void loop() {
    mqtt.loop();
    int i;
    bool p_cooling = false;
    bool p_heating = false;

    /* notify the HVAC and servo value every 1 sec */
    if (++notifCaller >= 1000) {
        for (i = 0; i < NO_OF_ROOMS; i++) {

            /* establish if room suppose to be heated or cooled */
            if(!isWindowsOpen(room, i) == true && room[i].isTermometerActive == true) {
                if (room[i].heating == false && room[i].tempReal <= (room[i].heatingTemp - room[i].hyst)) 
                    room[i].heating = true;
                if (room[i].heating == true && room[i].tempReal >= (room[i].heatingTemp + room[i].hyst))
                    room[i].heating = false;
                if (room[i].cooling == false && room[i].tempReal >= (room[i].coolingTemp + room[i].hyst))
                    room[i].cooling = true;
                if (room[i].cooling == true && room[i].tempReal <= (room[i].coolingTemp - room[i].hyst))
                    room[i].cooling = false;
            } else {
                room[i].heating = false;
                room[i].cooling = false;
            }
      
            /* set the Valve (servo) due to heating/cooling condition */
            set_valve(room[i].ValveHeating_id, room[i].heating);
            set_valve(room[i].ValveCooling_id, room[i].cooling);
       
            /* set LED indicator for Valve (servo) */
            digitalWrite(ledPIN_fromServoID(room[i].ValveHeating_id), room[i].ValveHeating_isOpen);
            digitalWrite(ledPIN_fromServoID(room[i].ValveCooling_id), room[i].ValveCooling_isOpen);

            /* establish Peltier status */
            if(room[i].heating)
                p_heating = true;
            if(room[i].cooling)
                p_cooling = true;

            /* notify HVAC Rooms status by MQTT */
            mqtt.send(output_JSON_HVACRooms(room, i));
        }

        /* set Peltier */
        digitalWrite(PELTIER_COOLING, p_cooling);
        digitalWrite(PELTIER_HEATING, p_heating);
    
        mqtt.send(output_JSON_HVACStatus(p_cooling, p_heating));

        /* optional preview in Serial monitor */
        #if HVAC_DEBUG==1
        for (i = 0; i < NO_OF_ROOMS; i++) {
            show_data_SP(room, i);
        }
        #endif  

        notifCaller = 0;
    }
    
    delay(1);
}
