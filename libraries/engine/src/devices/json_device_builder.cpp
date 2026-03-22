#include "json_device_builder.hpp"

#include "../exceptions/data_exceptions.hpp"
#include "../exceptions/files_exceptions.hpp"
#include "../exceptions/devices_exceptions.hpp"

#include <ArduinoJson.h>
#include <SD.h>

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace
{
class JsonConfiguredDevice : public BaseDevice
{
public:
    explicit JsonConfiguredDevice(const std::string &uid) : BaseDevice(uid) {}

    void init() override {}

    void setAllowedPinsCsv(const std::string &allowedPins)
    {
        AllowedPins = allowedPins;
    }
};

std::string toLowerCopy(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::string jsonVariantToString(JsonVariantConst value)
{
    if (value.is<const char *>()) {
        const char *text = value.as<const char *>();
        return text ? std::string(text) : std::string();
    }

    if (value.is<bool>()) {
        return value.as<bool>() ? "1" : "0";
    }

    if (value.is<long long>()) {
        return std::to_string(value.as<long long>());
    }

    if (value.is<unsigned long long>()) {
        return std::to_string(value.as<unsigned long long>());
    }

    if (value.is<float>() || value.is<double>()) {
        std::ostringstream out;
        out << std::setprecision(8) << value.as<double>();
        return out.str();
    }

    return "";
}

std::string jsonVariantToCsv(JsonVariantConst value)
{
    if (value.is<JsonArrayConst>()) {
        std::string csv;
        for (JsonVariantConst item : value.as<JsonArrayConst>()) {
            const std::string itemValue = jsonVariantToString(item);
            if (itemValue.empty()) {
                continue;
            }

            if (!csv.empty()) {
                csv += ",";
            }
            csv += itemValue;
        }
        return csv;
    }

    return jsonVariantToString(value);
}

DeviceDataType parseDeviceDataType(const std::string &dtype)
{
    const std::string normalized = toLowerCopy(dtype);
    if (normalized == "int") {
        return DeviceDataType::INT;
    }
    if (normalized == "float") {
        return DeviceDataType::FLOAT;
    }
    if (normalized == "double") {
        return DeviceDataType::DOUBLE;
    }
    if (normalized == "string") {
        return DeviceDataType::STRING;
    }

    throw InvalidDataTypeException("parseDeviceDataType", "Unknown dtype: " + dtype);
}

DeviceRole parseDeviceRole(const std::string &role)
{
    const std::string normalized = toLowerCopy(role);
    if (normalized.empty() || normalized == "sensor") {
        return DeviceRole::SENSOR;
    }
    if (normalized == "actuator") {
        return DeviceRole::ACTUATOR;
    }
    if (normalized == "hybrid") {
        return DeviceRole::HYBRID;
    }

    throw InvalidDataFormatException("parseDeviceRole", "Unknown device role: " + role);
}

DeviceRestrictions parseRestrictions(JsonObjectConst restrictionsJson)
{
    DeviceRestrictions restrictions;

    if (!restrictionsJson["min"].isNull()) {
        restrictions.Min = jsonVariantToString(restrictionsJson["min"]);
    }
    if (!restrictionsJson["max"].isNull()) {
        restrictions.Max = jsonVariantToString(restrictionsJson["max"]);
    }
    if (!restrictionsJson["step"].isNull()) {
        restrictions.Step = jsonVariantToString(restrictionsJson["step"]);
    }
    if (!restrictionsJson["options"].isNull()) {
        restrictions.Options = jsonVariantToCsv(restrictionsJson["options"]);
    }

    return restrictions;
}

DeviceParam parseParameter(JsonObjectConst paramJson, JsonVariantConst defaultValue)
{
    DeviceParam param {};
    param.Value = "0";
    param.Unit = "";
    param.DType = DeviceDataType::STRING;
    param.lastHistoryIndex = 0;

    if (!paramJson["value"].isNull()) {
        param.Value = jsonVariantToString(paramJson["value"]);
    }
    if (!defaultValue.isNull()) {
        param.Value = jsonVariantToString(defaultValue);
    }
    if (!paramJson["unit"].isNull()) {
        param.Unit = jsonVariantToString(paramJson["unit"]);
    }
    if (!paramJson["dtype"].isNull()) {
        param.DType = parseDeviceDataType(jsonVariantToString(paramJson["dtype"]));
    }
    if (paramJson["restrictions"].is<JsonObjectConst>()) {
        param.Restrictions = parseRestrictions(paramJson["restrictions"].as<JsonObjectConst>());
    }

    for (int i = 0; i < HISTORY_CAP; ++i) {
        param.History[i] = param.Value;
    }

    return param;
}

void populateValueParameters(JsonConfiguredDevice &sensor, JsonObjectConst paramsJson, JsonObjectConst defaultsJson)
{
    for (JsonPairConst pair : paramsJson) {
        if (!pair.value().is<JsonObjectConst>()) {
            continue;
        }

        const std::string key = pair.key().c_str();
        JsonVariantConst defaultValue = defaultsJson[key];
        sensor.addValueParameter(key, parseParameter(pair.value().as<JsonObjectConst>(), defaultValue));
    }
}

void populateConfigParameters(JsonConfiguredDevice &sensor, JsonObjectConst paramsJson, JsonObjectConst defaultsJson)
{
    for (JsonPairConst pair : paramsJson) {
        if (!pair.value().is<JsonObjectConst>()) {
            continue;
        }

        const std::string key = pair.key().c_str();
        JsonVariantConst defaultValue = defaultsJson[key];
        sensor.addConfigParameter(key, parseParameter(pair.value().as<JsonObjectConst>(), defaultValue));
    }
}

JsonConfiguredDevice *buildConfiguredDevice(JsonObjectConst sensorJson, const std::string &sensorKey)
{
    const std::string uid = !sensorJson["uid"].isNull()
        ? jsonVariantToString(sensorJson["uid"])
        : sensorKey;

    if (uid.empty()) {
        throw DeviceInitializationFailException("buildConfiguredDevice", "Device uid is missing.");
    }

    if (sensorJson["type"].isNull()) {
        throw DeviceInitializationFailException("buildConfiguredDevice", "Device type is missing for " + uid + ".");
    }

    if (!sensorJson["values"].is<JsonObjectConst>() && !sensorJson["configs"].is<JsonObjectConst>()) {
        throw DeviceInitializationFailException("buildConfiguredDevice", "Device " + uid + " has neither values nor configs.");
    }

    JsonConfiguredDevice *sensor = new JsonConfiguredDevice(uid);
    try {
        sensor->Type = jsonVariantToString(sensorJson["type"]);
        sensor->Description = !sensorJson["description"].isNull()
            ? jsonVariantToString(sensorJson["description"])
            : "";
        sensor->setRole(!sensorJson["role"].isNull()
            ? parseDeviceRole(jsonVariantToString(sensorJson["role"]))
            : DeviceRole::SENSOR);

        const JsonObjectConst defaultsJson = sensorJson["default"].is<JsonObjectConst>()
            ? sensorJson["default"].as<JsonObjectConst>()
            : JsonObjectConst();

        const JsonObjectConst defaultValues = defaultsJson["values"].is<JsonObjectConst>()
            ? defaultsJson["values"].as<JsonObjectConst>()
            : JsonObjectConst();

        const JsonObjectConst defaultConfigs = defaultsJson["configs"].is<JsonObjectConst>()
            ? defaultsJson["configs"].as<JsonObjectConst>()
            : JsonObjectConst();

        if (sensorJson["values"].is<JsonObjectConst>()) {
            populateValueParameters(*sensor, sensorJson["values"].as<JsonObjectConst>(), defaultValues);
        }

        if (sensorJson["configs"].is<JsonObjectConst>()) {
            populateConfigParameters(*sensor, sensorJson["configs"].as<JsonObjectConst>(), defaultConfigs);
        }

        if (!sensorJson["allowedPins"].isNull()) {
            sensor->setAllowedPinsCsv(jsonVariantToCsv(sensorJson["allowedPins"]));
        } else if (!sensorJson["allowed_pins"].isNull()) {
            sensor->setAllowedPinsCsv(jsonVariantToCsv(sensorJson["allowed_pins"]));
        }

        if (!defaultsJson["pins"].isNull()) {
            const std::string pinsCsv = jsonVariantToCsv(defaultsJson["pins"]);
            std::stringstream pinsStream(pinsCsv);
            std::string pin;
            while (std::getline(pinsStream, pin, ',')) {
                if (!pin.empty()) {
                    sensor->assignPin(pin);
                }
            }
        }

        return sensor;
    } catch (...) {
        delete sensor;
        throw;
    }
}
}

DeviceCatalog buildDeviceCatalogFromSdFile(const std::string &filePath)
{
    if (filePath.empty()) {
        throw FileNotFoundException("buildDeviceCatalogFromSdFile", "Empty JSON path.");
    }

    if (!SD.exists(filePath.c_str())) {
        throw FileNotFoundException("buildDeviceCatalogFromSdFile", "Device DB not found on SD: " + filePath);
    }

    File file = SD.open(filePath.c_str(), FILE_READ);
    if (!file) {
        throw FileReadException("buildDeviceCatalogFromSdFile", "Cannot open device DB on SD: " + filePath);
    }

    JsonDocument doc;

    const DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        throw InvalidDataFormatException("buildDeviceCatalogFromSdFile", "JSON parsing failed: " + std::string(error.c_str()));
    }

    if (!doc["devices"].is<JsonObjectConst>()) {
        throw InvalidDataFormatException("buildDeviceCatalogFromSdFile", "Device DB is missing top-level 'devices' object.");
    }

    DeviceCatalog catalog;
    if (!doc["version"].isNull()) {
        catalog.version = jsonVariantToString(doc["version"]);
    }
    if (!doc["application"].isNull()) {
        catalog.application = jsonVariantToString(doc["application"]);
    }

    try {
        JsonObjectConst devicesJson = doc["devices"].as<JsonObjectConst>();
        for (JsonPairConst pair : devicesJson) {
            if (!pair.value().is<JsonObjectConst>()) {
                continue;
            }

            catalog.devices.push_back(buildConfiguredDevice(pair.value().as<JsonObjectConst>(), pair.key().c_str()));
        }
    } catch (...) {
        for (BaseDevice *device : catalog.devices) {
            delete device;
        }
        catalog.devices.clear();
        throw;
    }

    if (catalog.devices.empty()) {
        throw DeviceInitializationFailException("buildDeviceCatalogFromSdFile", "Device DB does not contain any buildable device.");
    }

    return catalog;
}
