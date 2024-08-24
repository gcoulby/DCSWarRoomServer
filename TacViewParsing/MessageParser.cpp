#include "pch.h"
#include "MessageParser.h"
#include "Types.h"
#include <regex>

// PUBLIC METHODS
MessageParser::MessageParser() {}
MessageParser::~MessageParser() {}


std::vector<std::string> MessageParser::getIds() {
	std::vector<std::string> ids;
	for (auto& [id, props] : entities) {
		ids.push_back(id);
	}
	return ids;
}


//void MessageParser::parseMessage(const std::string& input, DataMessages& entities, double* timeFrame) {
void MessageParser::parseMessage(const std::string& input) {
    // Escape special characters in the entire message first

    std::string escapedLine = escapeSpecialCharacters(input);
    //std::string normalizedLine = normalizeString(escapedLine);

    // Use a stringstream to split the escaped line by newlines
    std::istringstream stream(escapedLine);
    std::string line;
    while (std::getline(stream, line)) {

        auto tf = parseTimeFrame(line);
        if (tf.has_value()) {
            timeFrame = tf.value();
            continue;
        }

        auto header = parseHeaderMessage(line);
        if (header.has_value()) {
            entities["0"][header.value().first] = header.value().second;
            continue;
        }

        auto data = parseDataMessage(line);
        if (data.has_value()) {
            for (auto& [key, value] : data.value().properties) {

                if (key == "T") {
                    auto coords = parseObjectCoordinates(value);
                    if (coords.has_value()) {
                        for (auto& [key, value] : coords.value().properties) {
                            entities[data.value().id][key] = value;
                        }
                    }
                    continue;
                }


                entities[data.value().id][key] = value;
            }
        }
    }
}

// PRIVATE METHODS

std::string MessageParser::escapeSpecialCharacters(const std::string& text) {
    std::string result;
    size_t i = 0; // Use an index to allow checking the next character
    while (i < text.length()) {
        if (text[i] == '\\' && i + 1 < text.length()) { // Ensure there is a character to check after the '\\'
            if (text[i + 1] == '\n') {
                result += R"(\@)"; 
                i += 2; // Skip the next character to continue after the sequence
                continue; // Move to the next iteration
            }
            else if (text[i + 1] == ',') {
                result += R"(\*)";
                i += 2; // Skip the next character
                continue; // Move to the next iteration
            }
        }
        // If not a special sequence, add the current character and move to the next
        result.push_back(text[i]);
        i++;
    }
    return result;
}

std::string MessageParser::unescapeSpecialCharacters(const std::string& text) {
    std::string result;
    size_t i = 0; // Use an index to allow checking the next character
    while (i < text.length()) {
        if (text[i] == '\\' && i + 1 < text.length()) { // Ensure there is a character to check after the '\\'
            if (text[i + 1] == '@') {
                result += R"(\)";
                result += '\n';
                i += 2; // Skip the next character to continue after the sequence
                continue; // Move to the next iteration
            }
            else if (text[i + 1] == '*') {
                result += R"(\,)";
                i += 2; // Skip the next character
                continue; // Move to the next iteration
            }
        }
        // If not a special sequence, add the current character and move to the next
        result.push_back(text[i]);
        i++;
    }
    return result;
}


std::optional<double> MessageParser::parseTimeFrame(const std::string& timeFrame) {
    if (timeFrame.empty() || timeFrame[0] != '#') {
        return std::nullopt;
    }

    try {
        return std::stod(timeFrame.substr(1));
    }
    catch (const std::exception& e) { // Catching all std exceptions here for simplicity
        std::cerr << "Failed to parse time frame: " << e.what() << std::endl;
        return std::nullopt;
    }
}


std::vector<std::optional<std::string>> MessageParser::parseParts(const std::string& input) {
    std::istringstream iss(input);
    std::string part;
    std::vector<std::optional<std::string>> parts;

    // Parse the string into parts and attempt to convert them to doubles
    while (std::getline(iss, part, '|')) {
        if (part.empty()) {
            parts.push_back(std::nullopt); // Handle empty strings as no value
        }
        else {
            parts.push_back(part);
        }
    }

    return parts;
}


std::optional<DataMessage> MessageParser::parseObjectCoordinates(const std::string& coordString) {
	auto parts = parseParts(coordString);
	DataMessage coords;

    for (int i = 0; i < parts.size(); i++) {
		if (!parts[i].has_value()) {
			continue;
		}
        coords.properties[coordinates[i]] = parts[i].value();
	}

    return coords;
}


std::optional<std::pair<std::string, std::string>> MessageParser::parseHeaderMessage(const std::string& line) {
    std::regex pattern("^(FileType|FileVersion|ReferenceTime|RecordingTime|Title|DataRecorder|DataSource|Author|Comments|ReferenceLongitude|ReferenceLatitude)=");

    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            // Replace escaped newlines with actual newline characters
            size_t start_pos = 0;
            while ((start_pos = value.find("\\n", start_pos)) != std::string::npos) {
                value.replace(start_pos, 2, "\n");
                start_pos += 1; // Move past the replacement
            }

            value = unescapeSpecialCharacters(value);

            return { {key, value} };
        }
    }
    return std::nullopt;
}

std::optional<DataMessage> MessageParser::parseDataMessage(const std::string& line) {
    std::regex dataPattern(R"(^([a-f0-9]+),(.+))", std::regex_constants::icase);
    std::smatch matches;
    if (std::regex_match(line, matches, dataPattern) && matches.size() > 2) {
        DataMessage result;
        result.id = matches[1];
        std::string propertiesPart = matches[2];

        std::istringstream propStream(propertiesPart);
        std::string prop;
        while (std::getline(propStream, prop, ',')) {
            auto pos = prop.find('=');
            if (pos != std::string::npos) {
                std::string key = prop.substr(0, pos);
                std::string value = unescapeSpecialCharacters(prop.substr(pos + 1));
                result.properties[key] = value;
            }
        }
        return result;
    }
    return std::nullopt;
}

