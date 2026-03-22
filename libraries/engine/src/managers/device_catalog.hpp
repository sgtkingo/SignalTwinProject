#ifndef DEVICE_CATALOG_HPP
#define DEVICE_CATALOG_HPP

#include <string>
#include <vector>

#include "../devices/base_device.hpp"
#include "../devices/json_device_builder.hpp"

class DeviceCatalog
{
private:
    std::vector<BaseDevice *> devices;
    DeviceCatalogSchema schema;
    std::string configFilePath;
    std::string version;
    std::string application;
    bool initialized = false;

    void clear();

public:
    DeviceCatalog() = default;
    ~DeviceCatalog();

    bool init(const std::string &configFile = "");
    bool saveDraft(const DeviceDefinitionSchema &draft, const std::string &originalUid, bool isNewEntity);
    bool saveMetadata(const std::string &applicationValue, const std::string &versionValue);
    bool deleteDevice(const std::string &uid);
    bool isInitialized() const { return initialized; }

    BaseDevice *getDevice(const std::string &uid) const;
    const DeviceDefinitionSchema *getDeviceSchema(const std::string &uid) const;
    const std::vector<BaseDevice *> &getDevices() const { return devices; }
    const DeviceCatalogSchema &getSchema() const { return schema; }
    const std::string &getVersion() const { return version; }
    const std::string &getApplication() const { return application; }
    const std::string &getConfigFilePath() const { return configFilePath; }
};

#endif // DEVICE_CATALOG_HPP
