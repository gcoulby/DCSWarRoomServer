#include "pch.h"
#include "TacViewClient.h"



TacViewClient::TacViewClient(const std::string& host, const std::string& port, const std::string& password)
	: io_context_(), socket_(io_context_), resolver_(io_context_), password_(password)
{
	handshakeMessage = "XtraLib.Stream.0\nTacview.RealTimeTelemetry.0\nGenericClient\n" + password_ + '\0';
	tcp::resolver::query query(host, port);
	endpoint_ = resolver_.resolve(query);
}


TacViewClient::~TacViewClient()
{
	Disconnect();
}

void TacViewClient::Connect()
{
	asio::async_connect(socket_, endpoint_,
		[this](std::error_code ec, tcp::resolver::iterator) {
			if (!ec) {
				std::cout << "Connected to server!" << std::endl;
				SendHandshake();
				doRead();
			}
		});

	io_context_.run(); // Run the io_context object's event processing loop.
}

void TacViewClient::Disconnect()
{
	socket_.close();
}

void TacViewClient::SendHandshake()
{
	asio::async_write(socket_, asio::buffer(handshakeMessage),
		[this](std::error_code ec, std::size_t /* length */) {
			if (!ec) {
				std::cout << "Handshake message sent" << std::endl;
			}
		});
}

void TacViewClient::doRead() {
	socket_.async_read_some(asio::buffer(data_, max_length),
		[this](std::error_code ec, std::size_t length) {
			if (!ec) {
				if (OnDataReceived)
				{
					OnDataReceived(std::string(data_, length));
				}
				doRead();  // Continue reading data
			}
			else {
				std::cerr << "Read error: " << ec.message() << std::endl;
				Disconnect();
			}
		});
}

//void TacViewClient::onDataReceived(const std::string& data) {
//	//std::cout << "Received data from server: " << data << std::endl;
//	// Handle the data as needed, maybe parsing or further processing
//
//}