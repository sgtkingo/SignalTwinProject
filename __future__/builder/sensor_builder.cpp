/**
 * @file sensor_builder.cpp
 * @brief Implementation of sensor builder functions.
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#include "sensor_builder.hpp"
#include "../exceptions/sensors_exceptions.hpp"
#include "../exceptions/files_exceptions.hpp"
#include "../helpers.hpp"
#include <fstream>
#include <sstream>

DataType parseDataType(const std::string& dtypeStr) {
    if (dtypeStr == "int") return DataType::INT;
    if (dtypeStr == "float") return DataType::FLOAT;
    if (dtypeStr == "double") return DataType::DOUBLE;
    if (dtypeStr == "string") return DataType::STRING;
    
    throw InvalidDataTypeException("parseDataType", "Unknown data type: " + dtypeStr);
}

SensorRestrictions parseRestrictions(JsonObject restrictionsJson) {
    SensorRestrictions restrictions;
    
    if (restrictionsJson.containsKey("min")) {
        restrictions.Min = restrictionsJson["min"].as<std::string>();
    }
    if (restrictionsJson.containsKey("max")) {
        restrictions.Max = restrictionsJson["max"].as<std::string>();
    }
    if (restrictionsJson.containsKey("step")) {
        restrictions.Step = restrictionsJson["step"].as<std::string>();
    }
    if (restrictionsJson.containsKey("options")) {
        restrictions.Options = restrictionsJson["options"].as<std::string>();
    }
    
    return restrictions;
}

SensorParam parseParameter(JsonObject paramJson) {
    SensorParam param;
    
    // Parse value
    if (paramJson.containsKey("value")) {
        param.Value = paramJson["value"].as<std::string>();
    }
    
    // Parse unit
    if (paramJson.containsKey("unit")) {
        param.Unit = paramJson["unit"].as<std::string>();
    }
    
    // Parse data type
    if (paramJson.containsKey("dtype")) {
        std::string dtypeStr = paramJson["dtype"].as<std::string>();
        param.DType = parseDataType(dtypeStr);
    }
    
    // Initialize history
    param.lastHistoryIndex = 0;
    for (int i = 0; i < HISTORY_CAP; i++) {
        param.History[i] = param.Value;
    }
    
    // Parse restrictions if present
    if (paramJson.containsKey("restrictions")) {
        param.Restrictions = parseRestrictions(paramJson["restrictions"]);
    }
    
    return param;
}

bool validateSensorJson(JsonObject sensorJson, const std::string& sensorId) {
    // Check required fields
    if (!sensorJson.containsKey("uid")) {
        return false;
    }
    
    if (!sensorJson.containsKey("type")) {
        return false;
    }
    
    if (!sensorJson.containsKey("description")) {
        return false;
    }
    
    if (!sensorJson.containsKey("values")) {
        return false;
    }
    
    if (!sensorJson.containsKey("configs")) {
        return false;
    }
    
    return true;
}

BaseSensor* buildSensorFromJson(const std::string& jsonString, const std::string& sensorId) {
    try {
        // Parse JSON
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            throw InvalidDataFormatException("buildSensorFromJson", 
                "JSON parsing failed: " + std::string(error.c_str()));
        }
        
        // Check if sensors object exists
        if (!doc.containsKey("sensors")) {
            throw InvalidDataFormatException("buildSensorFromJson", 
                "JSON missing 'sensors' object");
        }
        
        JsonObject sensors = doc["sensors"];
        
        // Check if specific sensor exists
        if (!sensors.containsKey(sensorId)) {
            throw ValueNotFoundException("buildSensorFromJson", 
                "Sensor '" + sensorId + "' not found in JSON");
        }
        
        JsonObject sensorJson = sensors[sensorId];
        
        // Validate JSON structure
        if (!validateSensorJson(sensorJson, sensorId)) {
            throw InvalidConfigurationException("buildSensorFromJson", 
                "Invalid JSON structure for sensor: " + sensorId);
        }
        
        // Create sensor using VirtualSensor base class
        JsonConfiguredSensor* sensor = new JsonConfiguredSensor(sensorJson["uid"].as<std::string>());
        
        // Set basic properties
        sensor->Type = sensorJson["type"].as<std::string>();
        sensor->Description = sensorJson["description"].as<std::string>();
        sensor->Status = SensorStatus::OK;
        
        // Parse values
        JsonObject values = sensorJson["values"];
        for (JsonPair valuePair : values) {
            std::string key = valuePair.key().c_str();
            JsonObject valueObj = valuePair.value();
            
            try {
                SensorParam param = parseParameter(valueObj);
                sensor->addValueParameter(key, param);
            } catch (const Exception& e) {
                // Continue with other parameters if one fails
            }
        }
        
        // Parse configs
        JsonObject configs = sensorJson["configs"];
        for (JsonPair configPair : configs) {
            std::string key = configPair.key().c_str();
            JsonObject configObj = configPair.value();
            
            try {
                SensorParam param = parseParameter(configObj);
                sensor->addConfigParameter(key, param);
            } catch (const Exception& e) {
                // Continue with other parameters if one fails
            }
        }
        
        return sensor;
        
    } catch (const Exception& e) {
        throw;
    } catch (const std::exception& e) {
        throw InvalidConfigurationException("buildSensorFromJson", 
            "Failed to create sensor: " + std::string(e.what()));
    }
}

BaseSensor* buildSensorFromJsonFile(const std::string& filePath, const std::string& sensorId) {
    try {
        // Read file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw FileNotFoundException("buildSensorFromJsonFile", 
                "Cannot open file: " + filePath);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        std::string jsonString = buffer.str();
        
        // Use existing JSON string parser
        return buildSensorFromJson(jsonString, sensorId);
        
    } catch (const Exception& e) {
        throw;
    } catch (const std::exception& e) {
        throw FileReadException("buildSensorFromJsonFile", 
            "File reading error: " + std::string(e.what()));
    }
}

std::vector<std::string> getAvailableSensorIds(const std::string& jsonString) {
    std::vector<std::string> sensorIds;
    
    try {
        // Parse JSON
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, jsonString);
        
        if (error) {
            throw InvalidDataFormatException("getAvailableSensorIds", 
                "JSON parsing failed: " + std::string(error.c_str()));
        }
        
        // Check if sensors object exists
        if (!doc.containsKey("sensors")) {
            throw InvalidConfigurationException("getAvailableSensorIds", 
                "JSON missing 'sensors' object");
        }
        
        JsonObject sensors = doc["sensors"];
        
        // Collect all sensor IDs
        for (JsonPair sensorPair : sensors) {
            sensorIds.push_back(sensorPair.key().c_str());
        }
        
    } catch (const Exception& e) {
        throw;
    } catch (const std::exception& e) {
        throw InvalidDataFormatException("getAvailableSensorIds", 
            "Failed to parse sensor IDs: " + std::string(e.what()));
    }
    
    return sensorIds;
}

std::vector<std::string> getAvailableSensorIdsFromFile(const std::string& filePath) {
    try {
        // Read file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw FileNotFoundException("getAvailableSensorIdsFromFile", 
                "Cannot open file: " + filePath);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        std::string jsonString = buffer.str();
        
        // Use existing JSON string parser
        return getAvailableSensorIds(jsonString);
        
    } catch (const Exception& e) {
        throw;
    } catch (const std::exception& e) {
        throw FileReadException("getAvailableSensorIdsFromFile", 
            "File reading error: " + std::string(e.what()));
    }
}

std::vector<BaseSensor*> buildAllSensorsFromJson(const std::string& jsonString) {
    std::vector<BaseSensor*> sensors;
    
    try {
        // Get all available sensor IDs
        std::vector<std::string> sensorIds = getAvailableSensorIds(jsonString);
        
        // Build each sensor
        for (const std::string& sensorId : sensorIds) {
            try {
                BaseSensor* sensor = buildSensorFromJson(jsonString, sensorId);
                if (sensor) {
                    sensors.push_back(sensor);
                }
            } catch (const Exception& e) {
                // Log error but continue with other sensors
                // You might want to decide whether to continue or stop here
            }
        }
        
    } catch (const Exception& e) {
        // Clean up any successfully created sensors
        for (BaseSensor* sensor : sensors) {
            delete sensor;
        }
        sensors.clear();
        throw;
    }
    
    return sensors;
}

std::vector<BaseSensor*> buildAllSensorsFromJsonFile(const std::string& filePath) {
    try {
        // Read file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw FileNotFoundException("buildAllSensorsFromJsonFile", 
                "Cannot open file: " + filePath);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        std::string jsonString = buffer.str();
        
        // Use existing JSON string parser
        return buildAllSensorsFromJson(jsonString);
        
    } catch (const Exception& e) {
        throw;
    } catch (const std::exception& e) {
        throw FileReadException("buildAllSensorsFromJsonFile", 
            "File reading error: " + std::string(e.what()));
    }
}