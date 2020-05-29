/* Validation functions return false when requests are incorrect */

#ifndef REQUEST_VALIDATION_H
#define REQUEST_VALIDATION_H
#include "dependencies/json.hpp"

using json = nlohmann::json;

bool validate_light_request(json light_json){
  if(light_json.empty())
      return false;
  if(!(light_json.contains("hue")) && !(light_json.contains("saturation")) && !(light_json.contains("value")))
      return false;
  if((light_json.contains("hue"))){
      if(light_json["hue"].get<int>()<0)
          return false;
      if(light_json["hue"].get<int>()>359)
          return false;
  }
  if((light_json.contains("saturation"))){
      if(light_json["saturation"].get<int>()<0)
          return false;
      if(light_json["saturation"].get<int>()>100)
          return false;
  }
  if((light_json.contains("value"))){
      if(light_json["value"].get<int>()<0)
          return false;
      if(light_json["value"].get<int>()>100)
          return false;
  }
  return true;
}

bool validate_blind_request(json blinds_json){
  if(blinds_json.empty())
      return false;
  if(!(blinds_json.contains("position")))
      return false;
  if(blinds_json["position"].get<int>()<0)
      return false;
  if(blinds_json["position"].get<int>()>100)
      return false;
  return true;
}

bool validate_hvac_request(json hvac_json){
    if(hvac_json.empty())
        return false;
    if(!(hvac_json.contains("heatingTemperature")) && !(hvac_json.contains("coolingTemperature")) && !(hvac_json.contains("hysteresis")))
        return false;
    if((hvac_json.contains("heatingTemperature"))){
        if(hvac_json["heatingTemperature"].get<int>()<50)
            return false;
        if(hvac_json["heatingTemperature"].get<int>()>300)
            return false;
    }
    if((hvac_json.contains("coolingTemperature"))){
        if(hvac_json["coolingTemperature"].get<int>()<100)
            return false;
        if(hvac_json["coolingTemperature"].get<int>()>400)
            return false;
    }
    if((hvac_json.contains("hysteresis"))){
        if(hvac_json["hysteresis"].get<int>()<5)
            return false;
        if(hvac_json["hysteresis"].get<int>()>20)
            return false;
    }
    return true;
}
#endif /* REQUEST_VALIDATION_H */
