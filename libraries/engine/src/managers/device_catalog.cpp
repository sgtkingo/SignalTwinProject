#include "device_catalog.hpp"

#include "../devices/json_device_builder.hpp"
#include "expt.hpp"
#include "helpers.hpp"

#include <algorithm>

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
    schema = DeviceCatalogSchema();
    version.clear();
    application.clear();
    initialized = false;
}

bool DeviceCatalog::init(const std::string &configFile)
{
    clear();
    configFilePath = configFile.empty() ? DEFAULT_DEVICE_DB_PATH : configFile;

    logMessage("Initializing device catalog via JSON DB: %s\n", configFilePath.c_str());

    schema = parseDeviceCatalogSchemaFromSdFile(configFilePath);
    DeviceCatalogLoadResult catalog = buildDeviceCatalogFromSchema(schema);
    devices = std::move(catalog.devices);
    version = std::move(catalog.version);
    application = std::move(catalog.application);

    if (devices.empty()) {
        throw DeviceInitializationFailException("DeviceCatalog::init", "Device DB did not produce any devices.");
    }

    initialized = true;
    return true;
}

bool DeviceCatalog::saveDraft(const DeviceDefinitionSchema &draft, const std::string &originalUid, bool isNewEntity)
{
    DeviceCatalogSchema nextSchema = schema;
    if (nextSchema.version.empty()) {
        nextSchema.version = version;
    }
    if (nextSchema.application.empty()) {
        nextSchema.application = application;
    }

    const std::string targetOriginalUid = isNewEntity ? std::string() : originalUid;
    bool replaced = false;

    for (size_t i = 0; i < nextSchema.devices.size(); ++i) {
        DeviceDefinitionSchema &deviceSchema = nextSchema.devices[i];
        if (!targetOriginalUid.empty() && deviceSchema.uid == targetOriginalUid) {
            DeviceDefinitionSchema nextDraft = draft;
            nextDraft.sourceIndex = deviceSchema.sourceIndex;
            nextDraft.order = deviceSchema.order;
            nextDraft.hasExplicitOrder = deviceSchema.hasExplicitOrder;
            deviceSchema = nextDraft;
            replaced = true;
            continue;
        }

        if (deviceSchema.uid == draft.uid) {
            throw InvalidConfigurationException("DeviceCatalog::saveDraft", "Device UID already exists: " + draft.uid);
        }
    }

    if (!replaced) {
        DeviceDefinitionSchema nextDraft = draft;
        nextDraft.sourceIndex = nextSchema.devices.size();
        nextDraft.order = static_cast<int>(nextDraft.sourceIndex);
        nextDraft.hasExplicitOrder = false;
        nextSchema.devices.push_back(nextDraft);
    }

    DeviceCatalogLoadResult builtCatalog = buildDeviceCatalogFromSchema(nextSchema);

    try {
        saveDeviceCatalogSchemaToSdFile(nextSchema, configFilePath);
    } catch (...) {
        for (BaseDevice *device : builtCatalog.devices) {
            delete device;
        }
        throw;
    }

    for (BaseDevice *device : devices) {
        delete device;
    }

    devices = std::move(builtCatalog.devices);
    schema = std::move(nextSchema);
    version = schema.version;
    application = schema.application;
    initialized = true;
    return true;
}

bool DeviceCatalog::saveMetadata(const std::string &applicationValue, const std::string &versionValue)
{
    DeviceCatalogSchema nextSchema = schema;
    nextSchema.application = applicationValue;
    nextSchema.version = versionValue;

    DeviceCatalogLoadResult builtCatalog = buildDeviceCatalogFromSchema(nextSchema);

    try {
        saveDeviceCatalogSchemaToSdFile(nextSchema, configFilePath);
    } catch (...) {
        for (BaseDevice *device : builtCatalog.devices) {
            delete device;
        }
        throw;
    }

    for (BaseDevice *device : devices) {
        delete device;
    }

    devices = std::move(builtCatalog.devices);
    schema = std::move(nextSchema);
    version = schema.version;
    application = schema.application;
    initialized = true;
    return true;
}

bool DeviceCatalog::deleteDevice(const std::string &uid)
{
    DeviceCatalogSchema nextSchema = schema;
    auto it = std::find_if(nextSchema.devices.begin(), nextSchema.devices.end(), [&](const DeviceDefinitionSchema &deviceSchema) {
        return deviceSchema.uid == uid;
    });

    if (it == nextSchema.devices.end()) {
        throw InvalidConfigurationException("DeviceCatalog::deleteDevice", "Device not found: " + uid);
    }

    if (nextSchema.devices.size() <= 1) {
        throw InvalidConfigurationException("DeviceCatalog::deleteDevice", "Cannot delete the last device from the catalog.");
    }

    nextSchema.devices.erase(it);
    for (size_t i = 0; i < nextSchema.devices.size(); ++i) {
        nextSchema.devices[i].sourceIndex = i;
        nextSchema.devices[i].order = static_cast<int>(i);
        nextSchema.devices[i].hasExplicitOrder = false;
    }

    DeviceCatalogLoadResult builtCatalog = buildDeviceCatalogFromSchema(nextSchema);

    try {
        saveDeviceCatalogSchemaToSdFile(nextSchema, configFilePath);
    } catch (...) {
        for (BaseDevice *device : builtCatalog.devices) {
            delete device;
        }
        throw;
    }

    for (BaseDevice *device : devices) {
        delete device;
    }

    devices = std::move(builtCatalog.devices);
    schema = std::move(nextSchema);
    version = schema.version;
    application = schema.application;
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

const DeviceDefinitionSchema *DeviceCatalog::getDeviceSchema(const std::string &uid) const
{
    for (const DeviceDefinitionSchema &deviceSchema : schema.devices) {
        if (deviceSchema.uid == uid) {
            return &deviceSchema;
        }
    }

    return nullptr;
}
