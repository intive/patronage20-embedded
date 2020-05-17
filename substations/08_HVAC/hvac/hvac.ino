#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include <ArduinoJson.h>

#define SERVO_1H_LED 16
#define SERVO_1C_LED 5
#define SERVO_2H_LED 4
#define SERVO_2C_LED 0
#define PELTIER_HEATING 2
#define PELTIER_COOLING 14

/* HVAC Status */
const int hvacStatus_id = HVACStatus_ID;
const String hvacStatus_type = "HVACStatus";

/* HVAC Rooms  */
const String hvacRooms_type = "HVACRooms";

/* Room 1 - variables */
int r1_id = 1;
int r1_hyst = 0;
int r1_heatTemp = 0;
int r1_coolingTemp = 0;
int r1_cooling = 0;
int r1_heating = 0; 
int r1_temSenID = 0;
// int *winSeId = 0;

/* Room 2 - variables */
int r2_id = 2;
int r2_hyst = 0;
int r2_heatTemp = 0;
int r2_coolingTemp = 0;
int r2_cooling = 0;
int r2_heating = 0; 
int r2_temSenID = 0;
// int *winSeId = 0;

MQTT mqtt;
Network network(ssid, passwd);

void incoming_JSON(String json_input)
{
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    if (error) {
        return;
    }
    
    /* Room 1 */
    if(json_doc["id"].as<int>() == 1) {
        int r1_hyst = json_doc["hysteresis"].as<int>();
        int r1_heatTemp = json_doc["heatingTemperature"].as<int>();
        int r1_coolingTemp = json_doc["coolingTemperature"].as<int>();
        int r1_temSenID = json_doc["temperatureSensorId"].as<int>();
    }
    
    /* Room 2 */
    if(json_doc["id"].as<int>() == 2) {
        int r2_hyst = json_doc["hysteresis"].as<int>();
        int r2_heatTemp = json_doc["heatingTemperature"].as<int>();
        int r2_coolingTemp = json_doc["coolingTemperature"].as<int>();
        int r2_temSenID = json_doc["temperatureSensorId"].as<int>();
    }
}



String output_JSON_HVACStatus(bool c, bool h, int id, String t)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = t;
    root["cooling"] = c;
    root["heating"] = h;
    serializeJson(root, json_output);
    return json_output;
}

String output_JSON_HVACRooms(int id, int hyst, int heatTemp, int coolingTemp, int cooling, int heating, int temSenID,  int *winSeId)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = hvacRooms_type;
    root["coolingTemperature"] = heatTemp;
    root["heatingTemperature"] = coolingTemp;
    root["cooling"] = c;
    root["heating"] = h;
    serializeJson(root, json_output);
    return json_output;
}

void setup() {
    
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
  
  mqtt.send(output_JSON_HVACStatus(false, true, id, type));
  
  delay(5000);
  
  Serial.println(output_JSON_HVACStatus(false, true, id, type));
  mqtt.send(output_JSON_HVACStatus(true, false, id, type));

}
