#include "dependencies/crow_all.h"

#define GATEWAY_PORT 42893

int main()
{
    crow::SimpleApp gateway;
    
    CROW_ROUTE(gateway, "/hello")([](){
        return "Hello world";
    });

    CROW_ROUTE(gateway, "/dashboard").methods(crow::HTTPMethod::Get)([](){
        
        crow::json::wvalue dashboard_response;

        std::ifstream i("dashboard.json");
        std::stringstream dashboard_raw;
        dashboard_raw<<i.rdbuf();
        i.close();
        
        dashboard_response = crow::json::load(dashboard_raw.str());

        return crow::response(dashboard_response);
    });

    gateway.port(GATEWAY_PORT).multithreaded().run();
}
