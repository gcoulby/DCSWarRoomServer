#include "pch.h"
#include "RequestHandler.h"


http::response<http::string_body> RequestHandler::handle_request(const http::request<http::string_body>& req) {
    using namespace nlohmann;
    if (req.method() == http::verb::get && req.target() == "/api/data") {
        json json_response = { {"message", "This is a GET request"} };
        return create_response(req, http::status::ok, json_response.dump());
    }
    else if (req.method() == http::verb::post && req.target() == "/api/data") {
        json json_request = json::parse(req.body());
        json json_response = { {"message", "Received: " + json_request.dump()} };
        return create_response(req, http::status::ok, json_response.dump());
    }
    else if (req.method() == http::verb::put && req.target() == "/api/data") {
        json json_request = json::parse(req.body());
        json json_response = { {"message", "Updated: " + json_request.dump()} };
        return create_response(req, http::status::ok, json_response.dump());
    }
    else if (req.method() == http::verb::delete_ && req.target() == "/api/data") {
        json json_response = { {"message", "Resource deleted"} };
        return create_response(req, http::status::ok, json_response.dump());
    }
    return create_response(req, http::status::bad_request, "Unsupported method");
}

http::response<http::string_body> RequestHandler::create_response(const http::request<http::string_body>& req, http::status status, const std::string& body) {
    http::response<http::string_body> res{ status, req.version() };
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = body;
    res.prepare_payload();
    return res;
}
