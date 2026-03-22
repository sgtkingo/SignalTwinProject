#ifndef JSON_DEVICE_BUILDER_HPP
#define JSON_DEVICE_BUILDER_HPP

#include "base_device.hpp"

#include <string>
#include <vector>

struct DeviceCatalog
{
    std::vector<BaseDevice *> devices;
    std::string version;
    std::string application;
};

DeviceCatalog buildDeviceCatalogFromSdFile(const std::string &filePath);

#endif // JSON_DEVICE_BUILDER_HPP
