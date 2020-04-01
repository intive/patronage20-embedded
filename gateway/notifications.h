#include "dependencies/json.hpp"
#include <fstream>

using json = nlohmann::json;

enum type
{
    ALERT,
    DOORBELL,
    HVAC,
    WINDOWSENSOR,
    RFIDSENSOR
};

NLOHMANN_JSON_SERIALIZE_ENUM(type,{
    {ALERT,"alert"},
    {DOORBELL,"doorbell"},
    {HVAC,"hvac"},
    {WINDOWSENSOR,"windowSensor"},
    {RFIDSENSOR,"windowSensor"},
});

class Notifications
{
    json _notifications;

    public:

    Notifications(std::string filepath)
    {
        std::ifstream i(filepath);
        i >> _notifications;
        i.close();
    }

    json get_notifications()
    {
        return _notifications;
    }

    int delete_notification(int64_t id)
    {
        for (json::iterator it = _notifications.begin(); it != _notifications.end(); it++)
        {
            json j = it.value();
            if (j["id"] == id)
            {
                _notifications.erase(it);
                return 0;
            }
        }
        return 1;
    }
};