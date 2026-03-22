/**
 * @file sensor_builder.hpp
 * @brief Sensor builder for creating sensors from JSON configuration.
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef SENSOR_BUILDER_HPP
#define SENSOR_BUILDER_HPP

#include "virtual_sensor.hpp"
#include <ArduinoJson.h>
#include <string>
#include <vector>

/**
 * @brief JSON-configured implementation of VirtualSensor
 */
class JsonConfiguredDevice : public VirtualSensor {
public:
    JsonConfiguredDevice(std::string uid) : VirtualSensor() {
        this->UID = uid;
    }
    
    virtual ~JsonConfiguredDevice() {}
};

/**
 * @brief Convert JSON data type string to DataType enum
 * @param dtypeStr The data type string from JSON
 * @return DataType enum value
 */
DataType parseDataType(const std::string& dtypeStr);

/**
 * @brief Parse restrictions from JSON object
 * @param restrictionsJson JSON object containing restrictions
 * @return DeviceRestrictions struct
 */
DeviceRestrictions parseRestrictions(JsonObject restrictionsJson);

/**
 * @brief Parse sensor parameter from JSON
 * @param paramJson JSON object containing parameter data
 * @return DeviceParam struct
 */
DeviceParam parseParameter(JsonObject paramJson);

/**
 * @brief Validate JSON structure for required fields
 * @param sensorJson JSON object to validate
 * @param sensorId Sensor ID being processed
 * @return true if valid, false otherwise
 */
bool validateSensorJson(JsonObject sensorJson, const std::string& sensorId);

/**
 * @brief Build a sensor from JSON configuration
 * @param jsonString JSON string containing sensor configuration
 * @param sensorId ID of the sensor to build from the JSON
 * @return Pointer to created BaseDevice, nullptr on failure
 * @throws Exception if JSON parsing or sensor creation fails
 */
BaseDevice* buildSensorFromJson(const std::string& jsonString, const std::string& sensorId);

/**
 * @brief Build a sensor from JSON file
 * @param filePath Path to JSON file containing sensor configuration
 * @param sensorId ID of the sensor to build from the JSON
 * @return Pointer to created BaseDevice, nullptr on failure
 * @throws Exception if file reading, JSON parsing or sensor creation fails
 */
BaseDevice* buildSensorFromJsonFile(const std::string& filePath, const std::string& sensorId);

/**
 * @brief Get list of all sensor IDs available in JSON string
 * @param jsonString JSON string containing sensor configuration
 * @return Vector of sensor IDs
 * @throws Exception if JSON parsing fails
 */
std::vector<std::string> getAvailableSensorIds(const std::string& jsonString);

/**
 * @brief Get list of all sensor IDs available in JSON file
 * @param filePath Path to JSON file containing sensor configuration
 * @return Vector of sensor IDs
 * @throws Exception if file reading or JSON parsing fails
 */
std::vector<std::string> getAvailableSensorIdsFromFile(const std::string& filePath);

/**
 * @brief Build all sensors from JSON configuration
 * @param jsonString JSON string containing sensor configuration
 * @return Vector of pointers to created BaseDevices
 * @throws Exception if JSON parsing or sensor creation fails
 */
std::vector<BaseDevice*> buildAllSensorsFromJson(const std::string& jsonString);

/**
 * @brief Build all sensors from JSON file
 * @param filePath Path to JSON file containing sensor configuration
 * @return Vector of pointers to created BaseDevices
 * @throws Exception if file reading, JSON parsing or sensor creation fails
 */
std::vector<BaseDevice*> buildAllSensorsFromJsonFile(const std::string& filePath);

#endif // SENSOR_BUILDER_HPP