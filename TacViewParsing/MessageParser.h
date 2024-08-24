#pragma once
// Define export and import macros
#ifdef TACVIEWPARSING_EXPORTS
#define MESSAGEPARSER_API __declspec(dllexport)
#else
#define MESSAGEPARSER_API __declspec(dllimport)
#endif
#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <sstream>
#include "Types.h"

class MESSAGEPARSER_API  MessageParser
{
public:
    MessageParser();
    ~MessageParser();
    DataMessages entities;
    double timeFrame = 0;
    void parseMessage(const std::string& input);
    //void parseMessage(const std::string& input, DataMessages& entities, double* timeFrame = nullptr);
    std::vector<std::string> getIds();
    //static std::string normalizeString(const std::string& input);
private:
    std::optional<DataMessage> parseObjectCoordinates(const std::string& coordString);
    std::optional<std::pair<std::string, std::string>> parseHeaderMessage(const std::string& line);
    std::optional<DataMessage> parseDataMessage(const std::string& line);
    std::optional<double> parseTimeFrame(const std::string& timeFrame);
    std::string escapeSpecialCharacters(const std::string& text);
    std::string unescapeSpecialCharacters(const std::string& text);
    std::vector<std::optional<std::string>> parseParts(const std::string& input);
};

