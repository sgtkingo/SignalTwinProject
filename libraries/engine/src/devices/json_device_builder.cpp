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

constexpr const char *DEFAULT_SD_DB_PATH = "/data/DB.json";

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

const char *deviceRoleToString(DeviceRole role)
{
    switch (role) {
    case DeviceRole::ACTUATOR:
        return "actuator";
    case DeviceRole::HYBRID:
        return "hybrid";
    case DeviceRole::SENSOR:
    default:
        return "sensor";
    }
}

const char *deviceDataTypeToString(DeviceDataType dtype)
{
    switch (dtype) {
    case DeviceDataType::INT:
        return "int";
    case DeviceDataType::FLOAT:
        return "float";
    case DeviceDataType::DOUBLE:
        return "double";
    case DeviceDataType::STRING:
    default:
        return "string";
    }
}

void writeTypedJsonValue(JsonVariant target, const std::string &value, DeviceDataType dtype)
{
    try {
        switch (dtype) {
        case DeviceDataType::INT:
            target.set(convertStringToType<int>(value));
            return;
        case DeviceDataType::FLOAT:
            target.set(convertStringToType<float>(value));
            return;
        case DeviceDataType::DOUBLE:
            target.set(convertStringToType<double>(value));
            return;
        case DeviceDataType::STRING:
        default:
            target.set(value);
            return;
        }
    } catch (...) {
        target.set(value);
    }
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

std::vector<std::string> jsonVariantToStringVector(JsonVariantConst value)
{
    std::vector<std::string> items;

    if (value.is<JsonArrayConst>()) {
        for (JsonVariantConst item : value.as<JsonArrayConst>()) {
            const std::string itemValue = jsonVariantToString(item);
            if (!itemValue.empty()) {
                items.push_back(itemValue);
            }
        }
        return items;
    }

    const std::string single = jsonVariantToString(value);
    if (!single.empty()) {
        items.push_back(single);
    }
    return items;
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

int parseOrderValue(JsonObjectConst objectJson, size_t sourceIndex)
{
    if (!objectJson["order"].isNull()) {
        return objectJson["order"].as<int>();
    }
    return static_cast<int>(sourceIndex);
}

template <typename TSchema>
void sortByOrder(std::vector<TSchema> &items)
{
    std::stable_sort(items.begin(), items.end(), [](const TSchema &lhs, const TSchema &rhs) {
        if (lhs.order != rhs.order) {
            return lhs.order < rhs.order;
        }
        return lhs.sourceIndex < rhs.sourceIndex;
    });
}

std::vector<DeviceParamSchema> parseParameterSchemas(JsonObjectConst paramsJson, JsonObjectConst defaultsJson)
{
    std::vector<DeviceParamSchema> params;
    size_t sourceIndex = 0;

    for (JsonPairConst pair : paramsJson) {
        if (!pair.value().is<JsonObjectConst>()) {
            continue;
        }

        DeviceParamSchema schema;
        schema.key = pair.key().c_str();
        schema.sourceIndex = sourceIndex++;
        schema.hasExplicitOrder = !pair.value()["order"].isNull();
        schema.order = parseOrderValue(pair.value().as<JsonObjectConst>(), schema.sourceIndex);
        schema.param = parseParameter(pair.value().as<JsonObjectConst>(), defaultsJson[schema.key]);
        params.push_back(schema);
    }

    sortByOrder(params);
    return params;
}

std::vector<std::string> parseDefaultPins(JsonObjectConst defaultsJson)
{
    if (defaultsJson.isNull() || defaultsJson["pins"].isNull()) {
        return {};
    }

    if (defaultsJson["pins"].is<JsonArrayConst>()) {
        return jsonVariantToStringVector(defaultsJson["pins"]);
    }

    std::vector<std::string> pins;
    for (const std::string &pin : splitString(jsonVariantToString(defaultsJson["pins"]), ',')) {
        if (!pin.empty()) {
            pins.push_back(pin);
        }
    }
    return pins;
}

DeviceDefinitionSchema parseDeviceDefinitionSchema(JsonObjectConst deviceJson, const std::string &deviceKey, size_t sourceIndex)
{
    DeviceDefinitionSchema schema;
    schema.sourceIndex = sourceIndex;
    schema.hasExplicitOrder = !deviceJson["order"].isNull();
    schema.order = parseOrderValue(deviceJson, sourceIndex);
    schema.uid = !deviceJson["uid"].isNull() ? jsonVariantToString(deviceJson["uid"]) : deviceKey;

    if (schema.uid.empty()) {
        throw DeviceInitializationFailException("parseDeviceDefinitionSchema", "Device uid is missing.");
    }
    if (deviceJson["type"].isNull()) {
        throw DeviceInitializationFailException("parseDeviceDefinitionSchema", "Device type is missing for " + schema.uid + ".");
    }
    if (!deviceJson["values"].is<JsonObjectConst>() && !deviceJson["configs"].is<JsonObjectConst>()) {
        throw DeviceInitializationFailException("parseDeviceDefinitionSchema", "Device " + schema.uid + " has neither values nor configs.");
    }

    schema.type = jsonVariantToString(deviceJson["type"]);
    schema.description = !deviceJson["description"].isNull() ? jsonVariantToString(deviceJson["description"]) : "";
    schema.role = !deviceJson["role"].isNull()
        ? parseDeviceRole(jsonVariantToString(deviceJson["role"]))
        : DeviceRole::SENSOR;

    const JsonObjectConst defaultsJson = deviceJson["default"].is<JsonObjectConst>()
        ? deviceJson["default"].as<JsonObjectConst>()
        : JsonObjectConst();
    const JsonObjectConst defaultValues = defaultsJson["values"].is<JsonObjectConst>()
        ? defaultsJson["values"].as<JsonObjectConst>()
        : JsonObjectConst();
    const JsonObjectConst defaultConfigs = defaultsJson["configs"].is<JsonObjectConst>()
        ? defaultsJson["configs"].as<JsonObjectConst>()
        : JsonObjectConst();

    if (deviceJson["values"].is<JsonObjectConst>()) {
        schema.values = parseParameterSchemas(deviceJson["values"].as<JsonObjectConst>(), defaultValues);
    }
    if (deviceJson["configs"].is<JsonObjectConst>()) {
        schema.configs = parseParameterSchemas(deviceJson["configs"].as<JsonObjectConst>(), defaultConfigs);
    }

    if (!deviceJson["allowedPins"].isNull()) {
        schema.allowedPinsCsv = jsonVariantToCsv(deviceJson["allowedPins"]);
    } else if (!deviceJson["allowed_pins"].isNull()) {
        schema.allowedPinsCsv = jsonVariantToCsv(deviceJson["allowed_pins"]);
    }
    schema.defaultPins = parseDefaultPins(defaultsJson);

    return schema;
}

JsonConfiguredDevice *buildConfiguredDevice(const DeviceDefinitionSchema &schema)
{
    JsonConfiguredDevice *device = new JsonConfiguredDevice(schema.uid);
    try {
        device->Type = schema.type;
        device->Description = schema.description;
        device->setRole(schema.role);
        device->setAllowedPinsCsv(schema.allowedPinsCsv);

        for (const DeviceParamSchema &valueSchema : schema.values) {
            device->addValueParameter(valueSchema.key, valueSchema.param);
        }

        for (const DeviceParamSchema &configSchema : schema.configs) {
            device->addConfigParameter(configSchema.key, configSchema.param);
        }

        for (const std::string &pin : schema.defaultPins) {
            if (!pin.empty()) {
                device->assignPin(pin);
            }
        }

        return device;
    } catch (...) {
        delete device;
        throw;
    }
}

void writeRestrictions(JsonObject restrictionsJson, const DeviceRestrictions &restrictions)
{
    if (!restrictions.Min.empty()) {
        restrictionsJson["min"] = restrictions.Min;
    }
    if (!restrictions.Max.empty()) {
        restrictionsJson["max"] = restrictions.Max;
    }
    if (!restrictions.Step.empty()) {
        restrictionsJson["step"] = restrictions.Step;
    }
    if (!restrictions.Options.empty()) {
        JsonArray optionsJson = restrictionsJson["options"].to<JsonArray>();
        for (const std::string &option : splitString(restrictions.Options, ',')) {
            if (!option.empty()) {
                optionsJson.add(option);
            }
        }
    }
}

void writeParamsObject(JsonObject paramsJson, JsonObject defaultsJson, const std::vector<DeviceParamSchema> &params)
{
    for (const DeviceParamSchema &schema : params) {
        JsonObject paramJson = paramsJson[schema.key].to<JsonObject>();
        writeTypedJsonValue(paramJson["value"], schema.param.Value, schema.param.DType);
        paramJson["unit"] = schema.param.Unit;
        paramJson["dtype"] = deviceDataTypeToString(schema.param.DType);

        const bool hasRestrictions = !schema.param.Restrictions.Min.empty() ||
                                     !schema.param.Restrictions.Max.empty() ||
                                     !schema.param.Restrictions.Step.empty() ||
                                     !schema.param.Restrictions.Options.empty();
        if (hasRestrictions) {
            JsonObject restrictionsJson = paramJson["restrictions"].to<JsonObject>();
            writeRestrictions(restrictionsJson, schema.param.Restrictions);
        }

        writeTypedJsonValue(defaultsJson[schema.key], schema.param.Value, schema.param.DType);
    }
}
}

DeviceCatalogSchema parseDeviceCatalogSchemaFromSdFile(const std::string &filePath)
{
    std::string resolvedPath = filePath;
    if (resolvedPath.empty()) {
        resolvedPath = DEFAULT_SD_DB_PATH;
    }

    if (resolvedPath.empty()) {
        throw FileNotFoundException("buildDeviceCatalogFromSdFile", "Empty JSON path.");
    }

    if (!SD.exists(resolvedPath.c_str())) {
        throw FileNotFoundException("parseDeviceCatalogSchemaFromSdFile", "Device DB not found on SD: " + resolvedPath);
    }

    File file = SD.open(resolvedPath.c_str(), FILE_READ);
    if (!file) {
        throw FileReadException("parseDeviceCatalogSchemaFromSdFile", "Cannot open device DB on SD: " + resolvedPath);
    }

    JsonDocument doc;

    const DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        throw InvalidDataFormatException("parseDeviceCatalogSchemaFromSdFile", "JSON parsing failed: " + std::string(error.c_str()));
    }

    if (!doc["devices"].is<JsonObjectConst>()) {
        throw InvalidDataFormatException("parseDeviceCatalogSchemaFromSdFile", "Device DB is missing top-level 'devices' object.");
    }

    DeviceCatalogSchema catalog;
    if (!doc["version"].isNull()) {
        catalog.version = jsonVariantToString(doc["version"]);
    }
    if (!doc["application"].isNull()) {
        catalog.application = jsonVariantToString(doc["application"]);
    }

    try {
        JsonObjectConst devicesJson = doc["devices"].as<JsonObjectConst>();
        size_t sourceIndex = 0;
        for (JsonPairConst pair : devicesJson) {
            if (!pair.value().is<JsonObjectConst>()) {
                continue;
            }

            catalog.devices.push_back(parseDeviceDefinitionSchema(pair.value().as<JsonObjectConst>(), pair.key().c_str(), sourceIndex++));
        }
    } catch (...) {
        catalog.devices.clear();
        throw;
    }

    sortByOrder(catalog.devices);

    if (catalog.devices.empty()) {
        throw DeviceInitializationFailException("parseDeviceCatalogSchemaFromSdFile", "Device DB does not contain any buildable device.");
    }

    return catalog;
}

DeviceCatalogLoadResult buildDeviceCatalogFromSchema(const DeviceCatalogSchema &schemaCatalog)
{
    DeviceCatalogLoadResult catalog;
    catalog.version = schemaCatalog.version;
    catalog.application = schemaCatalog.application;

    try {
        for (const DeviceDefinitionSchema &deviceSchema : schemaCatalog.devices) {
            catalog.devices.push_back(buildConfiguredDevice(deviceSchema));
        }
    } catch (...) {
        for (BaseDevice *device : catalog.devices) {
            delete device;
        }
        catalog.devices.clear();
        throw;
    }

    return catalog;
}

DeviceCatalogLoadResult buildDeviceCatalogFromSdFile(const std::string &filePath)
{
    return buildDeviceCatalogFromSchema(parseDeviceCatalogSchemaFromSdFile(filePath));
}

bool saveDeviceCatalogSchemaToSdFile(const DeviceCatalogSchema &schemaCatalog, const std::string &filePath)
{
    std::string resolvedPath = filePath;
    if (resolvedPath.empty()) {
        resolvedPath = DEFAULT_SD_DB_PATH;
    }

    if (resolvedPath.empty()) {
        throw FileWriteException("saveDeviceCatalogSchemaToSdFile", "Empty JSON path.");
    }

    JsonDocument doc;
    doc["version"] = schemaCatalog.version;
    doc["application"] = schemaCatalog.application;
    JsonObject devicesJson = doc["devices"].to<JsonObject>();

    for (const DeviceDefinitionSchema &deviceSchema : schemaCatalog.devices) {
        JsonObject deviceJson = devicesJson[deviceSchema.uid].to<JsonObject>();
        deviceJson["uid"] = deviceSchema.uid;
        deviceJson["role"] = deviceRoleToString(deviceSchema.role);
        deviceJson["type"] = deviceSchema.type;
        deviceJson["description"] = deviceSchema.description;

        JsonObject valuesJson = deviceJson["values"].to<JsonObject>();
        JsonObject configsJson = deviceJson["configs"].to<JsonObject>();
        JsonObject defaultJson = deviceJson["default"].to<JsonObject>();
        JsonObject defaultValuesJson = defaultJson["values"].to<JsonObject>();
        JsonObject defaultConfigsJson = defaultJson["configs"].to<JsonObject>();

        writeParamsObject(valuesJson, defaultValuesJson, deviceSchema.values);
        writeParamsObject(configsJson, defaultConfigsJson, deviceSchema.configs);

        if (!deviceSchema.allowedPinsCsv.empty()) {
            JsonArray allowedPinsJson = deviceJson["allowedPins"].to<JsonArray>();
            for (const std::string &pin : splitString(deviceSchema.allowedPinsCsv, ',')) {
                if (!pin.empty()) {
                    try {
                        allowedPinsJson.add(convertStringToType<int>(pin));
                    } catch (...) {
                        allowedPinsJson.add(pin);
                    }
                }
            }
        }

        if (!deviceSchema.defaultPins.empty()) {
            JsonArray defaultPinsJson = defaultJson["pins"].to<JsonArray>();
            for (const std::string &pin : deviceSchema.defaultPins) {
                if (!pin.empty()) {
                    defaultPinsJson.add(pin);
                }
            }
        } else {
            defaultJson["pins"] = "";
        }
    }

    if (SD.exists(resolvedPath.c_str())) {
        SD.remove(resolvedPath.c_str());
    }

    File file = SD.open(resolvedPath.c_str(), FILE_WRITE);
    if (!file) {
        throw FileWriteException("saveDeviceCatalogSchemaToSdFile", "Cannot open device DB for writing: " + resolvedPath);
    }

    const size_t written = serializeJsonPretty(doc, file);
    file.close();

    if (written == 0) {
        throw FileWriteException("saveDeviceCatalogSchemaToSdFile", "Failed to serialize device DB: " + resolvedPath);
    }

    return true;
}
