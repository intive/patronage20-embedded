#include "dependencies/json.hpp"
#include "dashboard.h"
#include <fstream>
#include <ctime>

#define HVAC_ID -1
#define MAX_NOTIFICATION_COUNT 1024

using json = nlohmann::json;

struct Notification
{
    int64_t id;
    int64_t timestamp;
    std::string type;
    int sensorId;
};

long int unix_timestamp();
void to_json(json &j, const Notification &notification);
void from_json(const json &j, Notification &notification);

class Notifications
{
private:
    std::vector<Notification> notifications;
    Dashboard dashboard_previous;
    int64_t last_id;

public:
    void init(std::string filepath, const Dashboard &dashboard)
    {
        json j;
        std::ifstream i(filepath);
        if (i)
        {
            i >> j;
            i.close();
            for (json::iterator it = j.begin(); it != j.end(); it++)
            {
                notifications.push_back(it.value());
            }
            if (notifications.empty())
                last_id = 0;
            else
                last_id = notifications.back().id++;
        }
        else
        {
            i.close();
            std::ofstream o(filepath);
            o.close();
            last_id = 0;
        }
        
        dashboard_previous = dashboard;
    }

    json get_notifications()
    {

        return notifications;
    }

    void add_notification(int sensor_id, const std::string notification_type)
    {
        if (notifications.size() > MAX_NOTIFICATION_COUNT)
        {
            notifications.erase(notifications.begin());
        }
        Notification note;
        note.id = last_id++;
        note.timestamp = unix_timestamp();
        note.type = notification_type;
        note.sensorId = sensor_id;
        notifications.push_back(note);

        std::ofstream o("jsons/notifications.json");
        o.open("jsons/notifications.json");
        o << get_notifications();
        o.close();
    }

    int delete_notification(int64_t id)
    {
        for (auto i = notifications.begin(); i != notifications.end(); i++)
        {
            if (i->id == id)
            {
                notifications.erase(i);
                return 0;
            }
        }
        return 1;
    }

    int create_notification(json request, const Dashboard &dashboard)
    {

        /* WindowBlind  */
        if (request["type"] == "windowBlind")
        {
            for (uint i = 0; i < dashboard.window_blinds.size(); i++)
            {
                if (dashboard.window_blinds[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.window_blinds[i].position != request["position"].get<int>())
                    {
                        add_notification(dashboard.window_blinds[i].id, "windowBlind");
                        dashboard_previous.window_blinds[i].position = request["position"].get<int>();
                        return 0;
                    }
                }
            }
        }

        /* Lights */
        if (request["type"] == "LED_CONTROLLER")
        {
            for (uint i = 0; i < dashboard.lights.size(); i++)
            {
                if (dashboard.lights[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.lights[i].hue != request["hue"].get<int>() || dashboard_previous.lights[i].saturation != request["saturation"].get<int>() || dashboard_previous.lights[i].value != request["value"].get<int>())
                    {
                        add_notification(dashboard.lights[i].id, "led");
                        dashboard_previous.lights[i].hue = request["hue"].get<int>();
                        dashboard_previous.lights[i].saturation = request["saturation"].get<int>();
                        dashboard_previous.lights[i].value = request["value"].get<int>();
                        return 0;
                    }
                }
            }
        }

        /* WindowSensor */
        if (request["type"] == "windowSensor")
        {
            for (uint i = 0; i < dashboard.window_sensors.size(); i++)
            {
                if (dashboard.window_sensors[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.window_sensors[i].status != request["status"].get<int>())
                    {
                        add_notification(dashboard.window_sensors[i].id, "windowSensor");
                        dashboard_previous.window_sensors[i].status = request["status"].get<window_status>();
                        return 0;
                    }
                }
            }
        }

        /* smokeSensors */
        if (request["type"] == "smokeSensor")
        {
            for (uint i = 0; i < dashboard.smoke_sensors.size(); i++)
            {
                if (dashboard.smoke_sensors[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.smoke_sensors[i].is_smoke_detected != request["isSmokeDetected"].get<bool>())
                    {
                        add_notification(dashboard.smoke_sensors[i].id, "smokeSensor");
                        dashboard_previous.smoke_sensors[i].is_smoke_detected = request["isSmokeDetected"].get<bool>();
                        return 0;
                    }
                }
            }
        }

        /*HVAC status*/
        if (request["type"] == "HVACStatus")
        {
            for (uint i = 0; i < dashboard.hvac_status.size(); i++)
            {
                if (dashboard.hvac_status[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.hvac_status[i].cooling != request["cooling"].get<bool>() || dashboard_previous.hvac_status[i].heating != request["heating"].get<bool>())
                    {
                        add_notification(HVAC_ID, "hvacStatus");
                        dashboard_previous.hvac_status[i].cooling = request["cooling"].get<bool>();
                        dashboard_previous.hvac_status[i].heating = request["heating"].get<bool>();
                        return 0;
                    }
                }
            }
        }

        /*HVAC rooms*/
        if (request["type"] == "HVACRooms")
        {
            for (uint i = 0; i < dashboard.hvac_rooms.size(); i++)
            {
                if (dashboard.hvac_rooms[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.hvac_rooms[i].heating_temperature != request["heatingTemperature"].get<int>() ||
                        dashboard_previous.hvac_rooms[i].cooling_temperature != request["coolingTemperature"].get<int>() ||
                        dashboard_previous.hvac_rooms[i].hysteresis != request["hysteresis"].get<int>())
                    {
                        add_notification(dashboard.hvac_rooms[i].id, "hvacRoom");
                        dashboard_previous.hvac_rooms[i].heating_temperature = request["heatingTemperature"].get<int>();
                        dashboard_previous.hvac_rooms[i].cooling_temperature = request["coolingTemperature"].get<int>();
                        dashboard_previous.hvac_rooms[i].hysteresis = request["hysteresis"].get<int>();
                        return 0;
                    }
                }
            }
        }

        return 1;
    }
};

long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int>(t);
    return now;
}

/*Nlohmann json's methods for convertion between types and jsons*/
void to_json(json &j, const Notification &notification)
{
    j = json{
        {"id", notification.id},
        {"timestamp", notification.timestamp},
        {"type", notification.type},
        {"sensorId", notification.sensorId}};
}

void from_json(const json &j, Notification &notification)
{
    j.at("id").get_to(notification.id);
    j.at("timestamp").get_to(notification.timestamp);
    j.at("type").get_to(notification.type);
    j.at("sensorId").get_to(notification.sensorId);
}
