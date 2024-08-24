// BoostBeastServerTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define CROW_USE_BOOST


#include <boost/version.hpp>
#include <iostream>
#include <thread>
#include "crow_all.h"

int main()
{

    //crow::SimpleApp app;

    //CROW_ROUTE(app, "/")([]() {
    //    return "Hello world";
    //    });

    //app.port(18080).multithreaded().run();
    //
    //
    //std::cout << "Hello World!\n";


    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
        });

    // Create a thread to run the Crow server
    std::thread server_thread([&app]() {
        app.port(18080).multithreaded().run();
        });

    // Ensure the server thread is detached to run independently
    server_thread.detach();

    // Your main thread can execute other tasks
    std::cout << "Hello World!\n";

    // Prevent main from exiting if necessary or perform other tasks
    //server_thread.join(); // If you want to wait for the server to finish
    // Alternatively, you can have an infinite loop or a condition to keep checking or wait for a shutdown signal
     while (true) {
         std::this_thread::sleep_for(std::chrono::seconds(1));
     }

    return 0;
}

