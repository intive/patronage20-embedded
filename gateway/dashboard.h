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
    std::vector<HVACStatus> hvac_status;
    std::vector<HVACRoom> hvac_rooms;
    std::vector<Lights> lights;
    std::vector<Servo> servos;
    std::vector<MotionSensor> motion_sensors;

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
        hvac_status.push_back(status);
    }

    void add_hvac_room(HVACRoom room)
    {
        hvac_rooms.push_back(room);
    }

    void add_light(Lights light)
    {
        lights.push_back(light);
    }
    
    void add_servo(Servo servo)
    {
        servos.push_back(servo);
    }
    
    void add_motion_sensor(MotionSensor motion)
    {
        motion_sensors.push_back(motion);
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
    
    json get_dashboard_embedded()
    {
        json dashboard = 
        {
            {"servos",servos},
            {"motionSensors",motion_sensors}

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
        hvac_status = dashboard["HVACStatus"].get<std::vector<HVACStatus>>();
        hvac_rooms = dashboard["HVACRooms"].get<std::vector<HVACRoom>>();
        lights = dashboard["lights"].get<std::vector<Lights>>();
    }
    
    void set_dashboard_embedded(json dashboard)
    {
        servos = dashboard["servos"].get<std::vector<Servo>>();
        motion_sensors = dashboard["motionSensors"].get<std::vector<MotionSensor>>();
    }

    int set_blind(json request)
    {
        if(request["type"] != "windowBlind")
            return 1;
        else
            return 0;
    }

    int set_hvac_room(json request)
    {
        if(request["type"] != "HVACRoom")
            return 1;
        else
            return 0;
    }
    
    int set_light(json request)
    {
        if(request["type"] != "LED_CONTROLLER")
            return 1;
        else
            return 0;
    }
    
    int update_dashboard_embedded(json request)
    {
        int sensorExist = 0;
        
        /* Servos  */
        if(request["type"] == "Servo") 
        {
            for(uint i = 0;i<servos.size();i++)
            {
                if  (servos[i].id==(request["id"].get<int>()))
                {
                    servos[i].angle = request["angle"].get<int>();
                    sensorExist = 1;
                    return 0;
                } 
            }
            if (sensorExist == 0)
                {
                    add_servo(request);
                    return 0;
                }
 
        }
        /* motionSensors  */
        if(request["type"] == "motionSensor") 
        {
            for(uint i = 0;i<motion_sensors.size();i++)
            {
                if  (motion_sensors[i].id==(request["id"].get<int>()))
                {
                    motion_sensors[i].motion = request["motion"].get<bool>();
                    sensorExist = 1;
                    return 0;
                } 
            }
            if (sensorExist == 0)
                {
                    add_motion_sensor(request);
                    return 0;
                }
        }
    }
    
    int update_dashboard(json request)
    {
        int sensorExist = 0;
        
        /* WindowBlind  */
        if(request["type"] == "windowBlind") 
        {
            for(uint i = 0;i<window_blinds.size();i++)
            {
                if  (window_blinds[i].id==(request["id"].get<int>()))
                {
                    window_blinds[i].position = request["position"].get<int>();
                    sensorExist = 1;
                    return 0;
                }
            }
            if (sensorExist == 0) 
            {
                    add_window_blind(request);
                    return 0;
            }
        }
        
        /* Lights */
        if(request["type"] == "LED_CONTROLLER")
        {
            for(uint i = 0;i<lights.size();i++)
            {
                if  (lights[i].id==(request["id"].get<int>()))
                {
                    lights[i].hue = request["hue"].get<int>();
                    lights[i].saturation = request["saturation"].get<int>();
                    lights[i].value = request["value"].get<int>();
                    sensorExist = 1;
                    return 0;
                }
            }
            if (sensorExist == 0) 
            {
                add_light(request);
                return 0;
            }
        }
        
        /* WindowSensor */
        if(request["type"] == "windowSensor")
        {
            for(uint i = 0;i<window_sensors.size();i++)
            {
                if  (window_sensors[i].id==(request["id"].get<int>()))
                {
                    window_sensors[i].status = request["status"].get<window_status>();
                    sensorExist = 1;
                    return 0;
                }
            }
            if (sensorExist == 0) 
            {
                add_window_sensor(request);
                return 0;
            }
            
        }
        
        /* temperatureSensors */
        if(request["type"] == "TEMPERATURE_SENSOR")
        {
            for(uint i = 0;i<temperature_sensors.size();i++)
            {
                if  (temperature_sensors[i].id==(request["id"].get<int>()))
                {
                    temperature_sensors[i].value = request["value"].get<int>();
                    sensorExist = 1;
                    return 0;
                }
            }
            if (sensorExist == 0) 
            {
                add_temperature_sensor(request);
                return 0;
            }
            
            
        }
        
        /* HVAC Status */
        if(request["type"] == "HVACStatus")
        {
            for(uint i = 0;i<hvac_status.size();i++)
            {
                hvac_status[i].heating = request["heating"].get<bool>();
                hvac_status[i].cooling = request["cooling"].get<bool>();
                sensorExist = 1;
                return 0;
            }
        
            if (sensorExist == 0) 
            {
                add_hvac_sensor(request);
                return 0;
            }
        }
        
        /* HVAC Rooms */
        if(request["type"] == "HVACRooms")
        {
            for(uint i = 0;i<hvac_rooms.size();i++)
            {
                if  (hvac_rooms[i].id==request["id"].get<int>())
                    
                {
                    hvac_rooms[i].hysteresis = request["hysteresis"].get<int>();
                    hvac_rooms[i].heating_temperature = request["heatingTemperature"].get<int>();
                    hvac_rooms[i].cooling_temperature = request["coolingTemperature"].get<int>();
                    hvac_rooms[i].temperature_sensor_id = request["temperatureSensorId"].get<int>();
                    hvac_rooms[i].heating = request["heating"].get<int>();
                    hvac_rooms[i].cooling = request["cooling"].get<int>();
                    hvac_rooms[i].window_sensor_ids.clear();
                    for (uint j = 0; j < request["windowSensorIds"].size(); j++)
                    {
                        hvac_rooms[i].window_sensor_ids.push_back(request["windowSensorIds"][j].get<int>()); 
                    }
                return 0;
                }
                
            }
        }
        
        /* smokeSensors */
        if(request["type"] == "smokeSensor")
        {
            for(uint i = 0;i<smoke_sensors.size();i++)
            {
                if  (smoke_sensors[i].id==(request["id"].get<int>()))
                {
                    smoke_sensors[i].is_smoke_detected = request["isSmokeDetected"].get<bool>();
                    sensorExist = 1;
                    return 0;
                }
            }
            if (sensorExist == 0) 
            {
                add_smoke_sensor(request);
                return 0;
            }
        }

        return 1;
    }
};
