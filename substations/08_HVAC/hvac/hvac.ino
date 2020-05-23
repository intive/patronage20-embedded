/*
 * WARNING! At "init_config.h" - set the value:
 * #define HVAC_SUBST 1
 *
 */

#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include "hvac.h"
#include <ArduinoJson.h>

#define HVAC_DEBUG 1

unsigned int notifCaller = 0;
Network network(ssid, passwd);


void incoming_JSON(String json_input, char* topic)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    
    if (error)
        return;
        
    /* gather message from input channel */
    if (strcmp(topic, mqttInputTopic) == 0) 
        if(json_doc["type"].as<String>().equals("HVACRoom")) {
            updateRooms(room, json_doc);
            return;
        }
            
    /* gather message from output channel */
    if (strcmp(topic, mqttOutputTopic) == 0) {
        if (json_doc["type"].as<String>().equals("TEMPERATURE_SENSOR"))
            updateTermometer(room, json_doc);
        else if (json_doc["type"].as<String>().equals("Servo"))
            updateValve(room, json_doc);
        else if (json_doc["type"].as<String>().equals("windowSensor"))
            updateWindowsSens(json_doc);
    }

}

void setup() {

    /* set innitial values - different for Rooms */
    room[0].id = hvacRooms_R1_ID;
    room[0].valveHeating_id = SERVO_1H_ID;
    room[0].valveCooling_id = SERVO_1C_ID;
    room[1].id = hvacRooms_R2_ID;
    room[1].valveHeating_id = SERVO_2H_ID;
    room[1].valveCooling_id = SERVO_2C_ID;
    
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
    
    int i;
    bool p_cooling = false;
    bool p_heating = false;
    
    mqtt.loop();

    /* make establish, set and notify proper issues -  every 1 sec */
    if (++notifCaller >= 1000) {
        for (i = 0; i < NO_OF_ROOMS; i++) {

            /* establish if room suppose to be heated or cooled */
            if(!isWindowsOpen(room, i) == true && room[i].termometer_isActive == true) {
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
            set_valve(room[i].valveHeating_id, room[i].heating);
            set_valve(room[i].valveCooling_id, room[i].cooling);
       
            /* set LED indicator for Valve (servo) */
            digitalWrite(ledPIN_fromServoID(room[i].valveHeating_id), room[i].valveHeating_isOpen);
            digitalWrite(ledPIN_fromServoID(room[i].valveCooling_id), room[i].valveCooling_isOpen);

            /* establish Peltier status */
            if(room[i].heating)
                p_heating = true;
            if(room[i].cooling)
                p_cooling = true;

            /* notify HVAC Room status by MQTT */
            mqtt.send(output_JSON_HVACRooms(room, i));
        }

        /* set Peltier */
        digitalWrite(PELTIER_COOLING, p_cooling);
        digitalWrite(PELTIER_HEATING, p_heating);
    
        /* notify HVAC Status */
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
