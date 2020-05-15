#include "dependencies/json.hpp"
#include "dashboard.h"
#include <fstream>
#include <ctime>

#define MAX_ID 10
#define HVAC_ID -1
using json = nlohmann::json;

struct Notification
{
    int64_t id;
    int64_t timestamp;
    std::string type = "alert";
    int sensorId;
};

long int unix_timestamp();
void to_json(json &j, const Notification &notification);
void from_json(const json &j, Notification &notification);


class Notifications
{
private:
    json notifications;
    Dashboard dashboard_previous;
    int64_t last_id;

public:
    void init(std::string filepath, const Dashboard &dashboard)
    {
        std::ifstream i(filepath);
        i >> notifications;
        i.close();
        dashboard_previous = dashboard;
    }

    json get_notifications()
    {
        return notifications;
    }

    Notification create_notification(int64_t id, int sensor_id)
    {
        Notification note;
        note.id = id;
        note.timestamp = unix_timestamp();
        note.sensorId = sensor_id;
        return note;
    }

    void add_notification(Notification notification)
    {
        json j = notification;
        for (json::iterator it = notifications.begin(); it != notifications.end(); it++)
        {
            json temp = it.value();
            if (temp["id"] == notification.id)
            {
                notifications.erase(it);
                break;
            }
        }
        notifications.push_back(j);
        std:: ofstream o("jsons/notification.json");
        o.open("jsons/notification.json");
        o<<notifications;
        o.close();

    }

    int delete_notification(int64_t id)
    {
        for (json::iterator it = notifications.begin(); it != notifications.end(); it++)
        {
            json j = it.value();
            if (j["id"] == id)
            {
                notifications.erase(it);
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
                        add_notification(create_notification(set_new_id(), dashboard.window_blinds[i].id));
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
                        add_notification(create_notification(set_new_id(), dashboard.lights[i].id));
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
                        add_notification(create_notification(set_new_id(), dashboard.window_sensors[i].id));
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
                        add_notification(create_notification(set_new_id(), dashboard.smoke_sensors[i].id));
                        dashboard_previous.smoke_sensors[i].is_smoke_detected = request["isSmokeDetected"].get<bool>();
                        return 0;
                    }
                }
            }
        }

        /*HVAC status*/
        if (request["type"] == "smokeSensor")
        {
            for (uint i = 0; i < dashboard.smoke_sensors.size(); i++)
            {
                if (dashboard.hvac_status[i].id == (request["id"].get<int>()))
                {
                    if (dashboard_previous.hvac_status[i].cooling != request["cooling"].get<bool>() || dashboard_previous.hvac_status[i].heating != request["heating"].get<bool>())
                    {
                        add_notification(create_notification(set_new_id(), HVAC_ID));
                        dashboard_previous.hvac_status[i].cooling = request["cooling"].get<bool>();
                        dashboard_previous.hvac_status[i].heating = request["heating"].get<bool>();
                        return 0;
                    }
                }
            }
        }

        return 1;
    }

    int64_t set_new_id()
    {
        if(last_id<MAX_ID)
        {
            last_id++;
        }
        else
            last_id = 0;
        return last_id;
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
        {"sensorId", notification.sensorId}
    };
}

void from_json(const json &j, Notification &notification)
{
    j.at("id").get_to(notification.id);
    j.at("timestamp").get_to(notification.timestamp);
    j.at("type").get_to(notification.type);
    j.at("sensorId").get_to(notification.sensorId);
}
