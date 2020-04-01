#include "dependencies/crow_all.h"
#include "dependencies/json.hpp"
#include "dashboard.h"
#include "notifications.h"

#define GATEWAY_PORT 42893

using json = nlohmann::json;

class ExampleLogHandler : public crow::ILogHandler
{
    
public:
    void log(std::string message, crow::LogLevel level) override
    {
    }
};

struct DashboardMiddleware
{
    Dashboard dashboard;
    DashboardMiddleware()
    {
        std::ifstream i("dashboard.json");
        json dashboard_response;
        i>> dashboard_response;
        i.close();
        dashboard.set_dashboard(dashboard_response);
        
    };

    struct context
    {
    };
    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
    }

    void after_handle(crow::request &req, crow::response &res, context &ctx)
    {
    }
};

int main()
{

    crow::App<DashboardMiddleware> gateway;

    CROW_ROUTE(gateway, "/hello")
    ([]() {
        return "Hello world";
    });

    /*DASHBOARD*/
    CROW_ROUTE(gateway, "/dashboard").methods(crow::HTTPMethod::Get)([&]() {
        
        json dashboard = gateway.get_middleware<DashboardMiddleware>().dashboard.get_dashboard();

        crow::json::wvalue temp = crow::json::load(dashboard.dump());
        return crow::response(temp);
    });

    /*CONFIGURATION*/
    CROW_ROUTE(gateway, "/blinds").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json blind = json::parse(req.body);
        if (blind.empty())
            return crow::response(400);
        if(gateway.get_middleware<DashboardMiddleware>().dashboard.set_blind(blind))
            return crow::response(400);
        return crow::response(200);
    });

    CROW_ROUTE(gateway, "/hvac").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json hvac = json::parse(req.body);
        if (hvac.empty())
            return crow::response(400);
        if(gateway.get_middleware<DashboardMiddleware>().dashboard.set_hvac_room(hvac))
            return crow::response(400);
        return crow::response(200);
    });

    CROW_ROUTE(gateway, "/lights").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json light = json::parse(req.body);
        if (light.empty())
            return crow::response(404);
        if(gateway.get_middleware<DashboardMiddleware>().dashboard.set_light(light))
            return crow::response(400);
        return crow::response(200);
    });

    /*NOTIFICATIONS*/
    CROW_ROUTE(gateway, "/notifications").methods(crow::HTTPMethod::GET)([&]() {

        Notifications notifications("notifications.json");

        return crow::response(notifications.get_notifications().dump());
    });
    CROW_ROUTE(gateway, "/notifications/<int>").methods(crow::HTTPMethod::DELETE)([](int id) {
        Notifications notifications("notifications.json");
        if(notifications.delete_notification(id))
            return crow::response(400);
        std::ofstream o("notifications.json");
        o <<std::setw(4) << notifications.get_notifications()<<std::endl;
        o.close();

        return crow::response(200);
    });

    gateway.port(GATEWAY_PORT).multithreaded().run();
}
