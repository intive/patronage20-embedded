#include "dependencies/json.hpp"
#include "sensors.h"

using json = nlohmann::json;

class Dashboard
{
    std::vector<TemperatureSensor> temperature_sensors;
    std::vector<WindowSensor> window_sensors;
    std::vector<WindowBlind> window_blinds;
    std::vector<RFIDSensor> rfid_sensors;
    std::vector<SmokeSensor> smoke_sensors;
    HVACStatus hvac_status;
    std::vector<HVACRoom> hvac_rooms;
    std::vector<Lights> lights;

public:

    void add_temperature_sensor(TemperatureSensor sensor)
    {
        temperature_sensors.push_back(sensor);
    }

    void add_window_sensor(WindowSensor sensor)
    {
        window_sensors.push_back(sensor);
    }

    void add_window_blind(WindowBlind sensor)
    {
        window_blinds.push_back(sensor);
    }

    void add_rfid_sensor(RFIDSensor sensor)
    {
        rfid_sensors.push_back(sensor);
    }

    void add_smoke_sensor(SmokeSensor sensor)
    {
        smoke_sensors.push_back(sensor);
    }

    void add_hvac_sensor(HVACStatus status)
    {
        hvac_status = status;
    }

    void add_hvac_room(HVACRoom room)
    {
        hvac_rooms.push_back(room);
    }

    void add_light(Lights light)
    {
        lights.push_back(light);
    }

    json get_dashboard()
    {
        json dashboard = 
        {
            {"temperatureSensors", temperature_sensors},
            {"windowSensors",window_sensors},
            {"windowBlinds",window_blinds},
            {"RFIDSensors", rfid_sensors},
            {"smokeSensors",smoke_sensors},
            {"HVACStatus",hvac_status},
            {"HVACRooms",hvac_rooms},
            {"lights",lights}

        };
        return dashboard;
    }

    void set_dashboard(json dashboard)
    {
        temperature_sensors = dashboard["temperatureSensors"].get<std::vector<TemperatureSensor>>();
        window_sensors = dashboard["windowSensors"].get<std::vector<WindowSensor>>();
        window_blinds = dashboard["windowBlinds"].get<std::vector<WindowBlind>>();
        rfid_sensors = dashboard["RFIDSensors"].get<std::vector<RFIDSensor>>();
        smoke_sensors = dashboard["smokeSensors"].get<std::vector<SmokeSensor>>();
        hvac_status = dashboard["HVACStatus"].get<HVACStatus>();
        hvac_rooms = dashboard["HVACRooms"].get<std::vector<HVACRoom>>();
        lights = dashboard["lights"].get<std::vector<Lights>>();
    }

    int set_blind(json request)
    {
        if(request["type"] != "windowBlind")
            return 1;
        for(uint i = 0;i<window_blinds.size();i++)
        {
            if  (window_blinds[i].id==(request["id"].get<int>()))
            {
                window_blinds[i].position = request["position"].get<int>();
                return 0;
            }
        }
        return 1;
    }

    int set_hvac_room(json request)
    {
        if(request["type"] != "HVACRoom")
            return 1;
        for(uint i = 0;i<hvac_rooms.size();i++)
        {
            if  (hvac_rooms[i].id==(request["id"].get<int>()))
            {
                hvac_rooms[i].heating_temperature = request["heating-temperature"].get<int>();
                hvac_rooms[i].cooling_temperature = request["cooling-temperature"].get<int>();
                hvac_rooms[i].hysteresis = request["hysteresis"].get<int>();
                hvac_rooms[i].window_sensor_ids = request["windowSensorIds"].get<std::vector<int>>();
                return 0;
            }
        }
        return 1;
    }
    
    int set_light(json request)
    {
        if(request["type"] != "RGBLight")
            return 1;
        for(uint i = 0;i<lights.size();i++)
        {
            if  (lights[i].id==(request["id"].get<int>()))
            {
                lights[i].hue = request["hue"].get<int>();
                lights[i].saturation = request["saturation"].get<int>();
                lights[i].value = request["value"].get<int>();
                return 0;
            }
        }
        return 1;
    }
};
