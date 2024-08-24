#pragma once
#ifdef RESTAPILIB_EXPORTS
#define REQUESTHANDLER_API __declspec(dllexport)
#else
#define REQUESTHANDLER_API __declspec(dllimport)
#endif
#include <boost/beast/http.hpp>
#include "json.hpp"
#include <string>

namespace http = boost::beast::http;

class REQUESTHANDLER_API RequestHandler {
public:
    http::response<http::string_body> handle_request(const http::request<http::string_body>& req);
private:
    http::response<http::string_body> create_response(const http::request<http::string_body>& req, http::status status, const std::string& body);
};