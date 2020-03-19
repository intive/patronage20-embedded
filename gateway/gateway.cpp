#include "dependencies/crow_all.h"
#include "dependencies/json.hpp"

int main()
{
    crow::SimpleApp gateway;
    

    CROW_ROUTE(gateway, "/hello")([](){
        return "Hello world";
    });

    CROW_ROUTE(gateway, "/dashboard").methods(crow::HTTPMethod::Get)([](){
        
        crow::json::wvalue x;
        nlohmann::json j;

        std::ifstream i ("dashboard.json");
        i>>j;
        i.close();
        
        x = crow::json::load(j.dump());

        return crow::response(x);
    });

    gateway.port(42893).multithreaded().run();
}
