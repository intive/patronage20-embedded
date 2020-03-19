#include "dependencies/crow_all.h"
#include "dependencies/json.hpp"

#define GATEWAY_PORT 42893

int main()
{
    crow::SimpleApp gateway;
    
    CROW_ROUTE(gateway, "/hello")([](){
        return "Hello world";
    });

    CROW_ROUTE(gateway, "/dashboard").methods(crow::HTTPMethod::Get)([](){
        
        crow::json::wvalue dahsboard_response;
        nlohmann::json dashboard;

        std::ifstream i ("dashboard.json");
        i>>dashboard;
        i.close();
        
        dahsboard_response = crow::json::load(dashboard.dump());

        return crow::response(dahsboard_response);
    });

    gateway.port(GATEWAY_PORT).multithreaded().run();
}
