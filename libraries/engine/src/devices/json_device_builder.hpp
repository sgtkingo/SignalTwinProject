#ifndef JSON_DEVICE_BUILDER_HPP
#define JSON_DEVICE_BUILDER_HPP

#include "base_device.hpp"

#include <cstddef>
#include <string>
#include <vector>

struct DeviceParamSchema
{
    std::string key;
    DeviceParam param;
    size_t sourceIndex = 0;
    int order = 0;
    bool hasExplicitOrder = false;
};

struct DeviceDefinitionSchema
{
    std::string uid;
    std::string type;
    std::string description;
    std::string allowedPinsCsv;
    std::vector<std::string> defaultPins;
    DeviceRole role = DeviceRole::SENSOR;
    size_t sourceIndex = 0;
    int order = 0;
    bool hasExplicitOrder = false;
    std::vector<DeviceParamSchema> values;
    std::vector<DeviceParamSchema> configs;
};

struct DeviceCatalogSchema
{
    std::vector<DeviceDefinitionSchema> devices;
    std::string version;
    std::string application;
};

struct DeviceCatalog
{
    std::vector<BaseDevice *> devices;
    std::string version;
    std::string application;
};

DeviceCatalogSchema parseDeviceCatalogSchemaFromSdFile(const std::string &filePath);
DeviceCatalog buildDeviceCatalogFromSdFile(const std::string &filePath);

#endif // JSON_DEVICE_BUILDER_HPP
