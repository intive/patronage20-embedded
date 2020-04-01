#include "dependencies/json.hpp"
#include "sensors.h"

using json = nlohmann::json;

class Dashboard
{
    std::vector<TemperatureSensor> _temperature_sensors;
    std::vector<WindowSensor> _window_sensors;
    std::vector<WindowBlind> _window_blinds;
    std::vector<RFIDSensor> _rfid_sensors;
    std::vector<SmokeSensor> _smoke_sensors;
    HVACStatus _hvac_status;
    std::vector<HVACRoom> _hvac_rooms;
    std::vector<Lights> _lights;

public:
    /*Dashboard();
    Dashboard(json dashboard)
    {
    _temperature_sensors = dashboard["temperatureSensors"].get<std::vector<TemperatureSensor>>();
    _window_sensors = dashboard["windowSensors"].get<std::vector<WindowSensor>>();
    _window_blinds = dashboard["windowBlinds"].get<std::vector<WindowBlind>>();
    _rfid_sensors = dashboard["RFIDSensors"].get<std::vector<RFIDSensor>>();
    _smoke_sensors = dashboard["smokeSensors"].get<std::vector<SmokeSensor>>();
    _hvac_status = dashboard["HVACStatus"].get<HVACStatus>();
    _hvac_rooms = dashboard["HVACRooms"].get<std::vector<HVACRoom>>();
    _lights = dashboard["lights"].get<std::vector<Lights>>();
    };*/

    void add_temperature_sensor(TemperatureSensor sensor)
    {
        _temperature_sensors.push_back(sensor);
    }

    void add_window_sensor(WindowSensor sensor)
    {
        _window_sensors.push_back(sensor);
    }
    void add_window_blind(WindowBlind sensor)
    {
        _window_blinds.push_back(sensor);
    }
    void add_rfid_sensor(RFIDSensor sensor)
    {
        _rfid_sensors.push_back(sensor);
    }
    void add_smoke_sensor(SmokeSensor sensor)
    {
        _smoke_sensors.push_back(sensor);
    }
    void add_hvac_sensor(HVACStatus status)
    {
        _hvac_status = status;
    }
    void add_hvac_room(HVACRoom room)
    {
        _hvac_rooms.push_back(room);
    }
    void add_light(Lights light)
    {
        _lights.push_back(light);
    }

    json get_dashboard()
    {
        json dashboard = 
        {
            {"temperatureSensors", _temperature_sensors},
            {"windowSensors",_window_sensors},
            {"windowBlinds",_window_blinds},
            {"RFIDSensors", _rfid_sensors},
            {"smokeSensors",_smoke_sensors},
            {"HVACStatus",_hvac_status},
            {"HVACRooms",_hvac_rooms},
            {"lights",_lights}

        };
        return dashboard;
    }

    

    void set_dashboard(json dashboard)
    {
    _temperature_sensors = dashboard["temperatureSensors"].get<std::vector<TemperatureSensor>>();
    _window_sensors = dashboard["windowSensors"].get<std::vector<WindowSensor>>();
    _window_blinds = dashboard["windowBlinds"].get<std::vector<WindowBlind>>();
    _rfid_sensors = dashboard["RFIDSensors"].get<std::vector<RFIDSensor>>();
    _smoke_sensors = dashboard["smokeSensors"].get<std::vector<SmokeSensor>>();
    _hvac_status = dashboard["HVACStatus"].get<HVACStatus>();
    _hvac_rooms = dashboard["HVACRooms"].get<std::vector<HVACRoom>>();
    _lights = dashboard["lights"].get<std::vector<Lights>>();
    }

    int set_blind(json request)
    {
        if(request["type"] != "windowBlind")
            return 1;
        for(int i = 0;i<_window_blinds.size();i++)
        {
            if (_window_blinds[i]._id==(request["id"].get<int>()))
            {
                _window_blinds[i]._position = request["position"].get<int>();
                return 0;
            }
        }
        return 1;
    }

    int set_hvac_room(json request)
    {
        if(request["type"] != "HVACRoom")
            return 1;
        for(int i = 0;i<_hvac_rooms.size();i++)
        {
            if (_hvac_rooms[i]._id==(request["id"].get<int>()))
            {
                _hvac_rooms[i]._heating_temperature = request["heating-temperature"].get<int>();
                _hvac_rooms[i]._cooling_temperature = request["cooling-temperature"].get<int>();
                _hvac_rooms[i]._hysteresis = request["hysteresis"].get<int>();
                _hvac_rooms[i]._window_sensor_ids = request["windowSensorIds"].get<std::vector<int>>();
                return 0;
            }
        }
        return 1;
    }
    
    int set_light(json request)
    {
        if(request["type"] != "RGBLight")
            return 1;
        for(int i = 0;i<_lights.size();i++)
        {
            if (_lights[i]._id==(request["id"].get<int>()))
            {
                _lights[i]._hue = request["hue"].get<int>();
                _lights[i]._saturation = request["saturation"].get<int>();
                _lights[i]._value = request["value"].get<int>();
                return 0;
            }
        }
        return 1;
    }
    
    
};
