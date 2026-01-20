/*
* Copyright 2025 MTA
* Author: Ing. Jiri Konecny
*/

#include "helpers.hpp"

std::unordered_map<std::string, std::string> parseParamsFromString(const std::string& message, bool caseSensitive = false) {
    std::unordered_map<std::string, std::string> params;
    
    // Remove leading '?' if present
    std::string cleanMessage = message;
    if(!caseSensitive)
    {
        // Convert to lowercase
        std::transform(cleanMessage.begin(), cleanMessage.end(), cleanMessage.begin(), ::tolower);
    }

    if (!cleanMessage.empty() && cleanMessage[0] == '?') {
        cleanMessage = cleanMessage.substr(1);
    }
    
    // Split by '&' to get key-value pairs
    std::stringstream ss(cleanMessage);
    std::string pair;
    
    while (std::getline(ss, pair, '&')) {
        size_t equalPos = pair.find('=');
        if (equalPos != std::string::npos) {
            std::string key = pair.substr(0, equalPos);
            std::string value = pair.substr(equalPos + 1);
            params[key] = value;
        }
    }
    
    return params;
}

std::string getValueFromKeyValueLikeString(std::string str, std::string key, char separator = '&') {
    std::string value;
    size_t pos = str.find(key);
    if(pos != std::string::npos) {
        pos += key.length() + 1;
        size_t end = str.find(separator, pos);
        value = str.substr(pos, end - pos);
    }

    return value;
}

std::vector<std::string> splitString(std::string str, char separator) {
    std::vector<std::string> result;
    if (str.empty()) {
        return result;
    }

    size_t pos = 0;
    size_t end = 0;
    while((end = str.find(separator, pos)) != std::string::npos) {
        result.push_back(str.substr(pos, end - pos));
        pos = end + 1;
    }
    result.push_back(str.substr(pos));

    return result;
}

template <typename T>
T convertStringToType(const std::string &str) {
    throw std::invalid_argument("Unsupported type conversion");
}


// Specialization for int
template <>
int convertStringToType<int>(const std::string &str) {
    if (str.empty()) {
        return int(); // Return default-constructed int (0)
    }

    try
    {
        return std::stoi(str);
    }
    catch(const std::exception& e)
    {
        throw InvalidDataTypeException("convertStringToType<int>", str + " is non-int format string!");
    }
     
}

// Specialization for double
template <>
double convertStringToType<double>(const std::string &str) {
    if (str.empty()) {
        return double(); // Return default double (0.0)
    }

    try
    {
        return std::stod(str);
    }
    catch(const std::exception& e)
    {
        throw InvalidDataTypeException("convertStringToType<double>", str + " is non-double format string!");
    }
}

// Specialization for float
template <>
float convertStringToType<float>(const std::string &str) {
    if (str.empty()) {
        return float(); // Return default float (0.0f)
    }

    try
    {
        return std::stof(str);
    }
    catch(const std::exception& e)
    {
        throw InvalidDataTypeException("convertStringToType<float>", str + " is non-float format string!");
    }
}

// Specialization for std::string
template <>
std::string convertStringToType<std::string>(const std::string &str) {
    return str;
}

bool isInVector(const std::vector<std::string>& vec, const std::string& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}
