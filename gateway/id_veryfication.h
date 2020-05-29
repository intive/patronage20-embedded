/* Validation functions return true when IDs are correct */

#ifndef ID_VALIDATION_H
#define ID_VALIDATION_H
#define ID_HVAC 11
#define ID_LIGHTS 21
#define ID_MOTION_SENSOR 31
#define ID_RFID_SENSOR 41
#define ID_SMOKE_SENSOR 51
#define ID_ANALOG_TEMPERATURE_SENSOR 61
#define ID_DIGITAL_TEMPERATURE_SENSOR 62
#define ID_BLINDS 71
#define ID_LINEAR_WINDOW_SENSOR 81
#define ID_MAGNETIC_WINDOW_SENSOR 82

#include "dependencies/json.hpp"

using json = nlohmann::json;

bool validate_light_id(json light_json){
  if(light_json.empty() || !light_json.contains("id") || light_json["id"].get<int>()!=ID_LIGHTS)
      return false;
  return true;
}

bool validate_blind_id(json blinds_json){
  if(blinds_json.empty() || !blinds_json.contains("id") || blinds_json["id"].get<int>()!=ID_BLINDS)
      return false;
  return true;
}
bool validate_hvac_id(json hvac_json){
  if(hvac_json.empty() || !hvac_json.contains("id") || hvac_json["id"].get<int>()!=ID_HVAC)
      return false;
  return true;
}

bool validate_rfid_sensor_id(json rfid_json){
  if(rfid_json.empty() || !rfid_json.contains("id") || rfid_json["id"].get<int>()!=ID_RFID_SENSOR)
      return false;
  return true;
}

bool validate_motion_sensor_id(json motion_sensor){
  if(motion_sensor.empty() || !motion_sensor.contains("id") || motion_sensor["id"].get<int>()!=ID_MOTION_SENSOR)
      return false;
  return true;
}

bool validate_temperature_sensor_id(json temperature_sensor){
  if(temperature_sensor.empty() || !temperature_sensor.contains("id") || (temperature_sensor["id"].get<int>()!=ID_ANALOG_TEMPERATURE_SENSOR && temperature_sensor["id"].get<int>()!=ID_DIGITAL_TEMPERATURE_SENSOR))
      return false;
  return true;
}

bool validate_window_sensors_id(json window_sensor){
  if(window_sensor.empty() || !window_sensor.contains("id") || (window_sensor["id"].get<int>()!=ID_LINEAR_WINDOW_SENSOR && window_sensor["id"].get<int>()!=ID_MAGNETIC_WINDOW_SENSOR))
      return false;
  return true;
}

bool validate_smoke_sensor_id(json smoke_sensor){
  if(smoke_sensor.empty() || !smoke_sensor.contains("id") || smoke_sensor["id"].get<int>()!=ID_SMOKE_SENSOR)
      return false;
  return true;
}
#endif /* ID_VALIDATION_H */
