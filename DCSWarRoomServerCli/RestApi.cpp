#include "RestApi.h"


RestApi::RestApi(MessageParser* parser): parser(parser)
{

}

RestApi::~RestApi()
{
}	

void RestApi::Connect()
{
    if(parser == nullptr)
	{
		return;
	}
	// Create a connection to the DCS War Room Server
	// Connect to the DCS War Room Server
	// Send the data to the DCS War Room Server
	// Receive the response from the DCS War Room Server
	// Parse the response from the DCS War Room Server
	// Return the parsed response

    

    auto& cors = app.get_middleware<CORSHandler>();
    cors.global()
        .origin("http://localhost:8080")               // frontend vue-js host
        .allow_credentials()                           // just in case
        .methods("POST"_method, "GET"_method);


    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
        });

    CROW_ROUTE(app, "/api/v1/entities")([&parser](const crow::request& req) {
        try {

            //create json object to return
            crow::json::wvalue json;

            //get mode from query string
            auto mode_param = req.url_params.get("mode");


            //get ids from parser
            auto ids = parser->getIds();
            std::string mode = mode_param != nullptr ? mode_param : "basic";



            if (mode == "full") {
                for (const auto& id : ids) {
                    crow::json::wvalue entity;
                    for (const auto& [key, value] : parser->entities[id]) {
                        entity[key] = value;
                    }
                    json[id] = std::move(entity);
                }
            }
            else if (mode == "simple") {
                for (const auto& id : ids) {
                    crow::json::wvalue entity;
                    for (const auto& [key, value] : parser->entities[id]) {
                        if (key == "Name" || key == "Group")
                        {
                            entity[key] = value;
                        }
                    }
                    if (entity.size() > 0)
                    {
                        json[id] = std::move(entity);
                    }
                }
            }
            else {
                crow::json::wvalue entities;
                json["entities"]["ids"] = std::vector<std::string>(ids.begin(), ids.end());
                json["count"] = ids.size();
            }
            return crow::response(200, json.dump());
        }
        catch (std::exception& e) {
            return crow::response(500, e.what());
        }
        });

    CROW_ROUTE(app, "/api/v1/entities/<string>")([&parser](const std::string& id) {
        crow::json::wvalue entity;
        for (const auto& [key, value] : parser->entities[id]) {
            entity[key] = value;
        }

        return crow::response(200, entity.dump());
        });

    // Create a thread to run the Crow server
    std::thread server_thread([]() {
        app.port(18080).multithreaded().run();
    });

    // Ensure the server thread is detached to run independently
    server_thread.detach();
}