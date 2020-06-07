#define CROW_ENABLE_SSL
#include "dependencies/crow_all.h"
#include "notifications.h"
#include <mosquitto.h>
#include <pthread.h>
#include "request_validation.h"

#define CERT_FILE "/etc/letsencrypt/live/gate.patronage2020-iot.intive-projects.com/fullchain.pem"
#define KEY_FILE "/etc/letsencrypt/live/gate.patronage2020-iot.intive-projects.com/privkey.pem"
#define COOKIE_NAME "SuperToken"
#define COOKIE_VALUE "59c5f5b2cb7ca698b5b9dd199a10914dc6047ef1afe07d2879c89637fef05ae2"

#define JSON_DASHBOARD "jsons/dashboard.json"
#define JSON_DASHBOARD_EMBEDDED "jsons/dashboard_embedded.json"
#define JSON_NOTIFICATIONS "jsons/notifications.json"

#define CA_FILE "/etc/ssl/certs/ca-certificates.crt"
#define MQTT_HOST "gate.patronage2020-iot.intive-projects.com"
#define MQTT_PORT 8883
#define QOS 1

#define TOPIC_I "embedded_input"
#define TOPIC_O "embedded_output"

struct CookieProtection
{
    /** our lambda function **/
    std::function<crow::CookieParser::context &(crow::request &req)> lamGetCookieCtx;

    /** stores an enviroment that allows us to access the cookies context **/
    template <typename... Middlewares>
    void init(crow::Crow<Middlewares...> &app)
    {
        lamGetCookieCtx = [&](crow::request &req) -> crow::CookieParser::context & {
            return app.template get_context<crow::CookieParser>(req);
        };
    }

    Dashboard dashboard;
    Dashboard dashboard_embedded;
    Notifications notifications;

    /* load default data from file */
    CookieProtection()
    {
        std::ifstream i(JSON_DASHBOARD);
        json dashboard_response;
        i >> dashboard_response;
        i.close();
        dashboard.set_dashboard(dashboard_response);
        std::ifstream j(JSON_DASHBOARD_EMBEDDED);
        json dashboard_embedded_response;
        j >> dashboard_embedded_response;
        j.close();
        dashboard_embedded.set_dashboard_embedded(dashboard_embedded_response);
        notifications.init(JSON_NOTIFICATIONS, dashboard);
    };

    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        auto &cookiectx = lamGetCookieCtx(req);

        if (cookiectx.get_cookie(COOKIE_NAME) != COOKIE_VALUE)
        {
            res.code = 403;
            res.end();
            return;
        }
        else
        {
            CROW_LOG_INFO << "COOKIE OK";
        }
    }

    void after_handle(crow::request &, crow::response &res, context &ctx)
    {
    }
};

crow::App<crow::CookieParser, CookieProtection> app;
pthread_rwlock_t q_rwlock = PTHREAD_RWLOCK_INITIALIZER;
FILE *mutex_log;

#define MUTEX_RDLOCK do { \
    pthread_rwlock_rdlock(&q_rwlock); \
    fprintf(mutex_log, "MUTEX RDLOCK - %s:%d\n", __FILE__, __LINE__); \
    fflush(mutex_log); \
} while(0)

#define MUTEX_WRLOCK do { \
    pthread_rwlock_wrlock(&q_rwlock); \
    fprintf(mutex_log, "MUTEX WRLOCK - %s:%d\n", __FILE__, __LINE__); \
    fflush(mutex_log); \
} while(0)

#define MUTEX_UNLOCK do { \
    pthread_rwlock_unlock(&q_rwlock); \
    fprintf(mutex_log, "MUTEX UNLOCK - %s:%d\n", __FILE__, __LINE__); \
    fflush(mutex_log); \
} while(0)

void mqtt_recv(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    char *incoming_msg = (char *)message->payload;

    json update = json::parse(incoming_msg);

    MUTEX_WRLOCK;
    app.get_middleware<CookieProtection>().dashboard_embedded.update_dashboard_embedded(update);
    app.get_middleware<CookieProtection>().dashboard.update_dashboard(update);
    app.get_middleware<CookieProtection>().notifications.create_notification(update, app.get_middleware<CookieProtection>().dashboard);

    MUTEX_UNLOCK;
}

int mqtt_send(struct mosquitto *mosq, const char *msg)
{

    return mosquitto_publish(mosq, NULL, TOPIC_I, strlen(msg), msg, QOS, 0);
}

int main(void)
{

    app.get_middleware<CookieProtection>().init(app);

    mosquitto_lib_init();
    mosquitto *mosq = mosquitto_new(NULL, true, NULL);

    if (!(mutex_log = fopen("mutex.log", "wt")))
        return 1;

    if (!mosq)
        return 1;

    if (mosquitto_tls_set(mosq, CA_FILE, NULL, NULL, NULL, NULL) != MOSQ_ERR_SUCCESS)
        return 1;

    if (mosquitto_connect_async(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
        return 1;

    mosquitto_message_callback_set(mosq, mqtt_recv);

    if (mosquitto_subscribe(mosq, NULL, TOPIC_O, 2) != MOSQ_ERR_SUCCESS)
        return 1;

    if (mosquitto_loop_start(mosq) != MOSQ_ERR_SUCCESS)
        return 1;

    CROW_ROUTE(app, "/")
    ([&](const crow::request &req) -> crow::response {
        CROW_LOG_INFO << "HELLO WORLD";
        return std::string("Hello world\n");
    });

    /* DASHBOARD */
    CROW_ROUTE(app, "/dashboard").methods(crow::HTTPMethod::Get)([&]() {
        MUTEX_RDLOCK;
        json dashboard = app.get_middleware<CookieProtection>().dashboard.get_dashboard();
        MUTEX_UNLOCK;
        crow::json::wvalue temp = crow::json::load(dashboard.dump());
        return crow::response(temp);
    });

    /* DASHBOARD Embedded */
    CROW_ROUTE(app, "/embedded").methods(crow::HTTPMethod::Get)([&]() {
        MUTEX_RDLOCK;
        json dashboard = app.get_middleware<CookieProtection>().dashboard_embedded.get_dashboard_embedded();
        MUTEX_UNLOCK;
        crow::json::wvalue temp = crow::json::load(dashboard.dump());
        return crow::response(temp);
    });

    /*CONFIGURATION*/
    CROW_ROUTE(app, "/blinds").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json blind = json::parse(req.body);
        if (!validate_blind_request(blind))
            return crow::response(400);
        if(!validate_blind_id(blind))
            return crow::response(400);
        MUTEX_RDLOCK;
        if (app.get_middleware<CookieProtection>().dashboard.set_blind(blind))
        {
            MUTEX_UNLOCK;
            return crow::response(400);
        }
        MUTEX_UNLOCK;

        mqtt_send(mosq, blind.dump().c_str());

        return crow::response(200);
    });

    CROW_ROUTE(app, "/hvac").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json hvac = json::parse(req.body);
        if (!validate_hvac_request(hvac))
            return crow::response(400);
        if(!validate_hvac_id(hvac))
            return crow::response(400);
        MUTEX_RDLOCK;
        if (app.get_middleware<CookieProtection>().dashboard.set_hvac_room(hvac))
        {
            MUTEX_UNLOCK;
            return crow::response(400);
        }
        MUTEX_UNLOCK;

        mqtt_send(mosq, hvac.dump().c_str());

        return crow::response(200);
    });

    CROW_ROUTE(app, "/lights").methods(crow::HTTPMethod::PUT)([&](const crow::request &req) {
        json light = json::parse(req.body);

        if (!validate_light_request(light))
            return crow::response(400);
        if(!validate_light_id(light))
            return crow::response(400);
        MUTEX_RDLOCK;
        if (app.get_middleware<CookieProtection>().dashboard.set_light(light))
        {
            MUTEX_UNLOCK;
            return crow::response(400);
        }
        MUTEX_UNLOCK;

        mqtt_send(mosq, light.dump().c_str());

        return crow::response(200);
    });

    /*NOTIFICATIONS*/
    CROW_ROUTE(app, "/notifications").methods(crow::HTTPMethod::GET)([&]() {
        MUTEX_RDLOCK;
        auto response = crow::response(app.get_middleware<CookieProtection>().notifications.get_notifications().dump());
        MUTEX_UNLOCK;
        return response;
    });
    CROW_ROUTE(app, "/notifications/<int>").methods(crow::HTTPMethod::DELETE)([&](int id) {
        MUTEX_WRLOCK;
        if (app.get_middleware<CookieProtection>().notifications.delete_notification(id))
        {
            MUTEX_UNLOCK;
            return crow::response(404);
        }
        std::ofstream o(JSON_NOTIFICATIONS);
        o << std::setw(4) << app.get_middleware<CookieProtection>().notifications.get_notifications() << std::endl;
        o.close();
        MUTEX_UNLOCK;
        return crow::response(200);
    });

    app.port(443)
        .ssl_file(CERT_FILE, KEY_FILE)
        .multithreaded()
        .run();

    return 0;
}
