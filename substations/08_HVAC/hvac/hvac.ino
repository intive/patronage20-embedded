#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include "hvac.h"
#include <ArduinoJson.h>


/* HVAC Rooms  */
const int hvacRooms_R1_ID = 1;
const int hvacRooms_R2_ID = 2;

struct Room 
{
    int id;
    int hyst = 0;
    int heatTemp = 0;
    int coolingTemp = 0;
    bool cooling = false;
    bool heating = false; 
    int temSenID = 0;
}r1, r2;




Network network(ssid, passwd);

void incoming_JSON(String json_input)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    if (error) {
        return;
    }
    
    /* Room 1 */
    if(json_doc["id"].as<int>() == hvacRooms_R1_ID) {
        r1.hyst = json_doc["hysteresis"].as<int>();
        r1.heatTemp = json_doc["heatingTemperature"].as<int>();
        r1.coolingTemp = json_doc["coolingTemperature"].as<int>();
        r1.temSenID = json_doc["temperatureSensorId"].as<int>();
    }
    
    /* Room 2 */
    if(json_doc["id"].as<int>() == hvacRooms_R2_ID) {
        r2.hyst = json_doc["hysteresis"].as<int>();
        r2.heatTemp = json_doc["heatingTemperature"].as<int>();
        r2.coolingTemp = json_doc["coolingTemperature"].as<int>();
        r2.temSenID = json_doc["temperatureSensorId"].as<int>();
    }
}



void setup() {
  
    r1.id = hvacRooms_R1_ID;
    r2.id = hvacRooms_R2_ID;
    
    pinMode(16, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(14, OUTPUT);

    /* Starting SerialPort */
    Serial.begin(9600);
    network.init();
    mqtt.set_return_function(incoming_JSON);
}

void loop() {
  mqtt.loop();
  
  delay(5000);
  
  mqtt.send(output_JSON_HVACStatus(false, true));
  set_valve(SERVO_1H_ID, true);
  set_valve(SERVO_1C_ID, true);
  set_valve(SERVO_2H_ID, true);
  set_valve(SERVO_2C_ID, true);

  set_peltier(HEATING, false);
  set_peltier(COOLING, true);
  
  delay(5000);
  
  //Serial.println(output_JSON_HVACStatus(false, true));
  mqtt.send(output_JSON_HVACStatus(true, false));
  set_valve(SERVO_1H_ID, false);
  set_valve(SERVO_1C_ID, false);
  set_valve(SERVO_2H_ID, false);
  set_valve(SERVO_2C_ID, false);

  set_peltier(HEATING, true);
  set_peltier(COOLING, false);

}
