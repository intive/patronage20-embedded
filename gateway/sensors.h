#include "dependencies/json.hpp"

using json = nlohmann::json;

enum window_status
{
    OPEN,
    CLOSED
};

NLOHMANN_JSON_SERIALIZE_ENUM(window_status,{
    {OPEN,"open"},
    {CLOSED,"closed"}

});

struct TemperatureSensor
{
    /*Properties*/
    int _id;
    std::string _type = "temperatureSensor";
    int _value;
};

struct WindowSensor
{
    int _id;
    std::string _type = "windowSensor";
    window_status _status;
};

struct WindowBlind
{
    int _id;
    std::string _type = "windowBlind";
    int _position;
};

struct LastTag
{
    int64_t _id;
    std::string _type = "RFIDTag";
    int64_t _timestamp;
};

struct RFIDSensor
{
    int _id;
    std::string _type = "RFIDSensor";
    LastTag _last_tag;
};

struct SmokeSensor
{
    int _id;
    std::string _type = "smokeSensor";
    bool _is_smoke_detected;
 
};

struct HVACStatus
{
    std::string _type = "HVACStatus";
    bool _heating;
    bool _cooling;
};

struct HVACRoom
{
    int _id;
    std::string _type = "HVACRoom";
    int _heating_temperature;
    int _cooling_temperature;
    int _hysteresis;
    int _temperature_sensor_id;
    std::vector<int> _window_sensor_ids;
   
};

struct Lights
{
    int _id;
    std::string _type = "RGBLight";
    int _hue;
    int _saturation;
    int _value;
};

struct Servo
{
    int _id;
    std::string _type = "Servo";
    int _angle;
};

struct LightColor
{
    int _id;
    std::string _type = "lightColor";
    int _value;
};

struct Keyboard
{
    int _id;
    std::string _type = "keyboard";
    std::string _message;
};


void to_json(json& j, const TemperatureSensor& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"value", sensor._value}
    };
}
void to_json(json& j, const WindowSensor& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"status", sensor._status}
    };
}
void to_json(json& j, const WindowBlind& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"position", sensor._position}
    };
}
void to_json(json& j, const LastTag& tag)
{
    j = json{
        { "id", tag._id},
        {"type", tag._type},
        {"timestamp", tag._timestamp}
    };
}
void to_json(json& j, const RFIDSensor& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"lastTag", sensor._last_tag}
    };
}

void to_json(json& j, const SmokeSensor& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"isSmokeDetected", sensor._is_smoke_detected}
    };
}
void to_json(json& j, const HVACStatus& status)
{
    j = json{
        {"type", status._type},
        {"heating", status._heating},
        {"cooling", status._cooling}
    };
}
void to_json(json& j, const HVACRoom& room)
{
    j = json{
        { "id", room._id},
        {"type", room._type},
        {"heatingTemperature", room._heating_temperature},
        {"coolingTemperature", room._cooling_temperature},
        {"hysteresis", room._hysteresis},
        {"temperatureSensorId", room._temperature_sensor_id},
        {"coolingTemperature", room._cooling_temperature}
    };
}
void to_json(json& j, const Lights& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"hue", sensor._hue},
        {"saturation", sensor._saturation},
        {"value", sensor._value}
    };
}

void to_json(json& j, const Servo& sensor)
{
    j = json{
        { "id", sensor._id},
        {"type", sensor._type},
        {"angle", sensor._angle}
    };
}
void to_json(json& j, const LightColor& color)
{
    j = json{
        { "id", color._id},
        {"type", color._type},
        {"value", color._value}
    };
}
void to_json(json& j, const Keyboard& keyboard)
{
    j = json{
        { "id", keyboard._id},
        {"type", keyboard._type},
        {"message", keyboard._message}
    };
}



void from_json(const json& j,  TemperatureSensor& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("value").get_to(sensor._value);
}
void from_json(const json& j,  WindowSensor& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("status").get_to(sensor._status);
}
void from_json(const json& j,  WindowBlind& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("position").get_to(sensor._position);
}
void from_json(const json& j,  LastTag& tag)
{
    j.at("id").get_to(tag._id);
    //j.at("type").get_to(tag._type);
    j.at("timestamp").get_to(tag._timestamp);
}
void from_json(const json& j,  RFIDSensor& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("lastTag").get_to(sensor._last_tag);
}

void from_json(const json& j,  SmokeSensor& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("isSmokeDetected").get_to(sensor._is_smoke_detected);
}
void from_json(const json& j,  HVACStatus& status)
{
    j.at("type").get_to(status._type);
    //j.at ("heating").get_to( status._heating);
    j.at("cooling").get_to(status._cooling);
    
}
void from_json(const json& j,  HVACRoom& room)
{
    j.at("id").get_to(room._id);
    //j.at("type").get_to(room._type);
    j.at("heating-temperature").get_to( room._heating_temperature);
    j.at("cooling-temperature").get_to( room._cooling_temperature);
    j.at("hysteresis").get_to( room._hysteresis);
    j.at("temperatureSensorId").get_to( room._temperature_sensor_id);
    j.at("windowSensorIds").get_to( room._window_sensor_ids);
}

void from_json(const json& j,  Lights& sensor)
{
    j.at("id").get_to(sensor._id);
    //j.at("type").get_to(sensor._type);
    j.at("hue").get_to( sensor._hue);
    j.at("saturation").get_to( sensor._saturation);
    j.at("value").get_to( sensor._value);
}

void from_json(const json& j, Servo& servo)
{
    j.at("id").get_to(servo._id);
    //j.at("type").get_to(servo._type);
    j.at("angle").get_to(servo._angle);
}

void from_json(const json& j,  LightColor& color)
{
    j.at("id").get_to(color._id);
    //j.at("type").get_to(color._type);
    j.at("value").get_to(color._value);
}

void from_json(const json& j,  Keyboard& keyboard)
{
    j.at("id").get_to(keyboard._id);
    //j.at("type").get_to(keyboard._type);
    j.at("message").get_to(keyboard._message);
}
