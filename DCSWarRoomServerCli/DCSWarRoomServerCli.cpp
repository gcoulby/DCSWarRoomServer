#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "TacViewClient.h"
#include "MessageParser.h"
#include "Types.h"
#include "RestApi.h"

#define CROW_USE_BOOST
#include <boost/version.hpp>
#include <iostream>
#include <thread>
#include "crow_all.h"


using namespace crow;

// Function to trim whitespace from the start and end of a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, (last - first + 1));
}

// Function to load .conf file into a map
std::unordered_map<std::string, std::string> loadConf(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_map<std::string, std::string> config;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return config;
    }

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                config[trim(key)] = trim(value);
            }
        }
    }

    return config;
}

std::string dcsHost;
std::string dcsPort;
std::string dcsPassword;


MessageParser parser;


static App<CORSHandler> app;
static void startRestApi() {

    auto& cors = app.get_middleware<CORSHandler>();
    cors.global()
        .origin("http://localhost:8080")               // frontend vue-js host
        .allow_credentials()                           // just in case
        .methods("POST"_method, "GET"_method);


    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
    });

    CROW_ROUTE(app, "/api/v1/entities")([](const crow::request& req) {
        try {
    	
        //create json object to return
        crow::json::wvalue json;

        //get mode from query string
        auto mode_param = req.url_params.get("mode");

        
        //get ids from parser
        auto ids = parser.getIds(); 
        std::string mode = mode_param != nullptr ? mode_param : "basic";
        


        if (mode == "full") {
            for (const auto& id : ids) {
                crow::json::wvalue entity;
                for (const auto& [key, value] : parser.entities[id]) {
                    entity[key] = value;
                }
                json[id] = std::move(entity);
            }
    	}
        else if (mode == "simple") {
            for (const auto& id : ids) {
                crow::json::wvalue entity;
                for (const auto& [key, value] : parser.entities[id]) {
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

    CROW_ROUTE(app, "/api/v1/entities/<string>")([](const std::string& id) {
    	crow::json::wvalue entity;
    	for (const auto& [key, value] : parser.entities[id]) {
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


int main() {
    std::string filename = "settings.conf"; // Specify your .conf file path
    auto config = loadConf(filename);


    if(config.find("DCS_HOST") != config.end()) {
		dcsHost = config["DCS_HOST"];
	}
    if(config.find("DCS_PORT") != config.end()) {
        dcsPort = config["DCS_PORT"];
    }
    if(config.find("DCS_PASSWORD") != config.end()) {
        dcsPassword = config["DCS_PASSWORD"];
	}

    // connect to DCS server
    std::cout << "Connecting to DCS server at " << dcsHost << ":" << dcsPort << " with password " << dcsPassword << std::endl;


    // start the RestApi
    startRestApi();

    try {
        TacViewClient client(dcsHost, dcsPort, dcsPassword);

        //on data received
        client.OnDataReceived = [](std::string data) {
            parser.parseMessage(data);
		};
        client.Connect();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }  

    return 0;
}
