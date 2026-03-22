#include "device_catalog.hpp"

#include "../devices/json_device_builder.hpp"
#include "expt.hpp"
#include "helpers.hpp"

namespace
{
constexpr const char *DEFAULT_DEVICE_DB_PATH = "/data/DB.json";
}

DeviceCatalog::~DeviceCatalog()
{
    clear();
}

void DeviceCatalog::clear()
{
    for (BaseDevice *device : devices) {
        delete device;
    }

    devices.clear();
    version.clear();
    application.clear();
    initialized = false;
}

bool DeviceCatalog::init(const std::string &configFile)
{
    clear();
    configFilePath = configFile.empty() ? DEFAULT_DEVICE_DB_PATH : configFile;

    logMessage("Initializing device catalog via JSON DB: %s\n", configFilePath.c_str());

    DeviceCatalogLoadResult catalog = buildDeviceCatalogFromSdFile(configFilePath);
    devices = std::move(catalog.devices);
    version = std::move(catalog.version);
    application = std::move(catalog.application);

    if (devices.empty()) {
        throw DeviceInitializationFailException("DeviceCatalog::init", "Device DB did not produce any devices.");
    }

    initialized = true;
    return true;
}

BaseDevice *DeviceCatalog::getDevice(const std::string &uid) const
{
    for (BaseDevice *device : devices) {
        if (device && device->UID == uid) {
            return device;
        }
    }

    return nullptr;
}
