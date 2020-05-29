/* Validation functions return true when IDs are correct */

#ifndef ID_VALIDATION_H
#define ID_VALIDATION_H
#define ID_HVAC 10
#define ID_LIGHTS 20
#define ID_MOTION_SENSOR 30
#define ID_RFID_SENSOR 40
#define ID_SMOKE_SENSOR 50
#define ID_TEMPERATURE_SENSOR 60
#define ID_BLINDS 70
#define ID_WINDOW_SENSOR 80
#define NUMBER_OF_SENSORS 9

#include "dependencies/json.hpp"

using json = nlohmann::json;

bool validate_light_id(json light_json){
  if(light_json.empty() || !light_json.contains("id") || light_json["id"].get<int>()-ID_LIGHTS < 1|| light_json["id"].get<int>()-ID_LIGHTS > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_blind_id(json blinds_json){
  if(blinds_json.empty() || !blinds_json.contains("id") || blinds_json["id"].get<int>()-ID_BLINDS < 1 || blinds_json["id"].get<int>()-ID_BLINDS > NUMBER_OF_SENSORS)
      return false;
  return true;
}
bool validate_hvac_id(json hvac_json){
  if(hvac_json.empty() || !hvac_json.contains("id") || hvac_json["id"].get<int>()-ID_HVAC < 1 || hvac_json["id"].get<int>()-ID_HVAC > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_rfid_sensor_id(json rfid_json){
  if(rfid_json.empty() || !rfid_json.contains("id") || rfid_json["id"].get<int>()-ID_RFID_SENSOR < 1 || rfid_json["id"].get<int>()-ID_RFID_SENSOR > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_motion_sensor_id(json motion_sensor){
  if(motion_sensor.empty() || !motion_sensor.contains("id") || motion_sensor["id"].get<int>()-ID_MOTION_SENSOR < 1|| motion_sensor["id"].get<int>()-ID_MOTION_SENSOR > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_temperature_sensor_id(json temperature_sensor){
  if(temperature_sensor.empty() || !temperature_sensor.contains("id") || temperature_sensor["id"].get<int>()-ID_TEMPERATURE_SENSOR < 1 || temperature_sensor["id"].get<int>()-ID_TEMPERATURE_SENSOR > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_window_sensors_id(json window_sensor){
  if(window_sensor.empty() || !window_sensor.contains("id") || window_sensor["id"].get<int>()-ID_WINDOW_SENSOR < 1 || window_sensor["id"].get<int>()-ID_WINDOW_SENSOR > NUMBER_OF_SENSORS)
      return false;
  return true;
}

bool validate_smoke_sensor_id(json smoke_sensor){
  if(smoke_sensor.empty() || !smoke_sensor.contains("id") || smoke_sensor["id"].get<int>()-ID_SMOKE_SENSOR < 1 || smoke_sensor["id"].get<int>()-ID_SMOKE_SENSOR > NUMBER_OF_SENSORS)
      return false;
  return true;
}
#endif /* ID_VALIDATION_H */
