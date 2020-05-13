#include "dependencies/json.hpp"

using json = nlohmann::json;

enum window_status
{
    OPEN,
    CLOSED
};

NLOHMANN_JSON_SERIALIZE_ENUM(window_status,
{
    {OPEN, "open"},
    {CLOSED, "closed"}
});

struct TemperatureSensor
{
    /*Properties*/
    int id;
    std::string type = "temperatureSensor";
    int value;
};

struct WindowSensor
{
    int id;
    std::string type = "windowSensor";
    window_status status;
};

struct WindowBlind
{
    int id;
    std::string type = "windowBlind";
    int position;
};

struct LastTag
{
    int64_t id;
    std::string type = "RFIDTag";
    int64_t timestamp;
};

struct RFIDSensor
{
    int id;
    std::string type = "RFIDSensor";
    LastTag last_tag;
};

struct SmokeSensor
{
    int id;
    std::string type = "smokeSensor";
    bool is_smoke_detected;
};

struct HVACStatus
{
    std::string type = "HVACStatus";
    bool heating;
    bool cooling;
};

struct HVACRoom
{
    int id;
    std::string type = "HVACRoom";
    int heating_temperature;
    int cooling_temperature;
    int hysteresis;
    int temperature_sensor_id;
    std::vector<int> window_sensor_ids;
};

struct Lights
{
    int id;
    std::string type = "RGBLight";
    int hue;
    int saturation;
    int value;
};

struct Servo
{
    int id;
    std::string type = "Servo";
    int angle;
};

struct LightColor
{
    int id;
    std::string type = "lightColor";
    int value;
};

struct Keyboard
{
    int id;
    std::string type = "keyboard";
    std::string message;
};

void to_json(json &j, const TemperatureSensor &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"value", sensor.value}};
}

void to_json(json &j, const WindowSensor &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"status", sensor.status}};
}

void to_json(json &j, const WindowBlind &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"position", sensor.position}};
}

void to_json(json &j, const LastTag &tag)
{
    j = json{
        {"id", tag.id},
        {"type", tag.type},
        {"timestamp", tag.timestamp}};
}

void to_json(json &j, const RFIDSensor &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"lastTag", sensor.last_tag}};
}

void to_json(json &j, const SmokeSensor &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"isSmokeDetected", sensor.is_smoke_detected}};
}

void to_json(json &j, const HVACStatus &status)
{
    j = json{
        {"type", status.type},
        {"heating", status.heating},
        {"cooling", status.cooling}};
}

void to_json(json &j, const HVACRoom &room)
{
    j = json{
        {"id", room.id},
        {"type", room.type},
        {"heatingTemperature", room.heating_temperature},
        {"coolingTemperature", room.cooling_temperature},
        {"hysteresis", room.hysteresis},
        {"temperatureSensorId", room.temperature_sensor_id},
        {"windowSensorIds", room.window_sensor_ids}};
}

void to_json(json &j, const Lights &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"hue", sensor.hue},
        {"saturation", sensor.saturation},
        {"value", sensor.value}};
}

void to_json(json &j, const Servo &sensor)
{
    j = json{
        {"id", sensor.id},
        {"type", sensor.type},
        {"angle", sensor.angle}};
}

void to_json(json &j, const LightColor &color)
{
    j = json{
        {"id", color.id},
        {"type", color.type},
        {"value", color.value}};
}

void to_json(json &j, const Keyboard &keyboard)
{
    j = json{
        {"id", keyboard.id},
        {"type", keyboard.type},
        {"message", keyboard.message}};
}

void from_json(const json &j, TemperatureSensor &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("value").get_to(sensor.value);
}

void from_json(const json &j, WindowSensor &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("status").get_to(sensor.status);
}

void from_json(const json &j, WindowBlind &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("position").get_to(sensor.position);
}

void from_json(const json &j, LastTag &tag)
{
    j.at("id").get_to(tag.id);
    //j.at("type").get_to(tag.type);
    j.at("timestamp").get_to(tag.timestamp);
}

void from_json(const json &j, RFIDSensor &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("lastTag").get_to(sensor.last_tag);
}

void from_json(const json &j, SmokeSensor &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("isSmokeDetected").get_to(sensor.is_smoke_detected);
}

void from_json(const json &j, HVACStatus &status)
{
    j.at("type").get_to(status.type);
    //j.at ("heating").get_to( status.heating);
    j.at("cooling").get_to(status.cooling);
}

void from_json(const json &j, HVACRoom &room)
{
    j.at("id").get_to(room.id);
    //j.at("type").get_to(room.type);
    j.at("heating-temperature").get_to(room.heating_temperature);
    j.at("cooling-temperature").get_to(room.cooling_temperature);
    j.at("hysteresis").get_to(room.hysteresis);
    j.at("temperatureSensorId").get_to(room.temperature_sensor_id);
    j.at("windowSensorIds").get_to(room.window_sensor_ids);
}

void from_json(const json &j, Lights &sensor)
{
    j.at("id").get_to(sensor.id);
    //j.at("type").get_to(sensor.type);
    j.at("hue").get_to(sensor.hue);
    j.at("saturation").get_to(sensor.saturation);
    j.at("value").get_to(sensor.value);
}

void from_json(const json &j, Servo &servo)
{
    j.at("id").get_to(servo.id);
    //j.at("type").get_to(servo.type);
    j.at("angle").get_to(servo.angle);
}

void from_json(const json &j, LightColor &color)
{
    j.at("id").get_to(color.id);
    //j.at("type").get_to(color.type);
    j.at("value").get_to(color.value);
}

void from_json(const json &j, Keyboard &keyboard)
{
    j.at("id").get_to(keyboard.id);
    //j.at("type").get_to(keyboard.type);
    j.at("message").get_to(keyboard.message);
}
