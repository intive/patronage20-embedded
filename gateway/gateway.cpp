#include "dependencies/crow_all.h"
#include "dependencies/json.hpp"
#include "dashboard.h"
#include "notifications.h"

#define GATEWAY_PORT 443
#define CROW_ENABLE_SSL

#define CERT_FILE "/etc/letsencrypt/live/gate.patronage2020-iot.intive-projects.com/fullchain.pem"
#define KEY_FILE "/etc/letsencrypt/live/gate.patronage2020-iot.intive-projects.com/privkey.pem"
#define COOKIE_NAME "SuperToken"
#define COOKIE_VALUE "59c5f5b2cb7ca698b5b9dd199a10914dc6047ef1afe07d2879c89637fef05ae2"

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

    std::function<crow::CookieParser::context &(crow::request &req)> lamGetCookieCtx;

    template <typename... Middlewares>
    void init(crow::Crow<Middlewares...> &app)
    {
        lamGetCookieCtx = [&](crow::request &req) -> crow::CookieParser::context & {
            //CROW_LOG_INFO << "GET COOKIE CONTEXT";
            return app.template get_context<crow::CookieParser>(req);
        };
    }

    Dashboard dashboard;

    DashboardMiddleware()
    {
        std::ifstream i("dashboard.json");
        json dashboard_response;
        i >> dashboard_response;
        i.close();
        dashboard.set_dashboard(dashboard_response);
    };

    struct context
    {
    };
    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        auto &cookiectx = lamGetCookieCtx(req);
        cookiectx.set_cookie(COOKIE_NAME, "ad21");
    }

    void after_handle(crow::request &req, crow::response &res, context &ctx)
    {
        auto &cookiectx = lamGetCookieCtx(req);
        if (cookiectx.get_cookie(COOKIE_NAME) != COOKIE_VALUE)
            res = crow::response(403);
        else
        {
            CROW_LOG_INFO << "COOKIE OK";
        }
    }
};

int main()
{

    crow::App<crow::CookieParser, DashboardMiddleware> gateway;
    gateway.get_middleware<DashboardMiddleware>().init(gateway);

    CROW_ROUTE(gateway, "/")
    ([&]() {
        return "Hello world";
    });

    CROW_ROUTE(gateway, "/hello")
    ([&](const crow::request &req) {
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
        if (gateway.get_middleware<DashboardMiddleware>().dashboard.set_blind(blind))
            return crow::response(400);
        return crow::response(200);
    });

    CROW_ROUTE(gateway, "/hvac").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json hvac = json::parse(req.body);
        if (hvac.empty())
            return crow::response(400);
        if (gateway.get_middleware<DashboardMiddleware>().dashboard.set_hvac_room(hvac))
            return crow::response(400);
        return crow::response(200);
    });

    CROW_ROUTE(gateway, "/lights").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json light = json::parse(req.body);
        if (light.empty())
            return crow::response(404);
        if (gateway.get_middleware<DashboardMiddleware>().dashboard.set_light(light))
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
        if (notifications.delete_notification(id))
            return crow::response(400);
        std::ofstream o("notifications.json");
        o << std::setw(4) << notifications.get_notifications() << std::endl;
        o.close();

        return crow::response(200);
    });

    gateway.port(GATEWAY_PORT)
        .ssl_file(CERT_FILE, KEY_FILE)
        .multithreaded()
        .run();
}
