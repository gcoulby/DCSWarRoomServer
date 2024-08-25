#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <map>


//make array of strings
const std::string coordinates[9] = {
	"Longitude",
	"Latitude",
	"Altitude",
	"Roll",
	"Pitch",
	"Yaw",
	"U",
	"V",
	"Heading"
};

using Properties = std::map<std::string, std::string>;

struct DataMessage {
    std::string id;
    Properties properties;
};

using DataMessages = std::unordered_map<std::string, Properties>;
