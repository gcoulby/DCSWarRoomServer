#pragma once
// Define export and import macros
#ifdef TACVIEWPARSING_EXPORTS
#define TACVIEWCLIENT_API __declspec(dllexport)
#else
#define TACVIEWCLIENT_API __declspec(dllimport)
#endif
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <functional>
#include <memory>

namespace asio = boost::asio;

using asio::ip::tcp;
using namespace std::chrono_literals;
class TACVIEWCLIENT_API TacViewClient
{
public:

	TacViewClient(const std::string& host, const std::string& port, const std::string& password);
	~TacViewClient();
	void Connect();
	void Disconnect();
	std::function<void(std::string)> OnDataReceived;
private:
	void SendHandshake();
	void doRead();

	asio::io_context io_context_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::iterator endpoint_;
	std::string password_;
	std::string handshakeMessage;
	enum { max_length = 1024 };
	char data_[max_length];
};