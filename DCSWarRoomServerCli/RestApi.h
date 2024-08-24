#pragma once
#include "MessageParser.h"
#define CROW_USE_BOOST
#include <boost/version.hpp>
#include <iostream>
#include <thread>
#include "crow_all.h"


using namespace crow;


class RestApi
{
public:
	RestApi(MessageParser* parser);
	~RestApi();
	void Connect();

private:
	static App<CORSHandler> app;
	MessageParser* parser;
};

