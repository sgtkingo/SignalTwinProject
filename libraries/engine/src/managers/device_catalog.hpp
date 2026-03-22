#ifndef DEVICE_CATALOG_HPP
#define DEVICE_CATALOG_HPP

#include <string>
#include <vector>

#include "../devices/base_device.hpp"

class DeviceCatalog
{
private:
    std::vector<BaseDevice *> devices;
    std::string configFilePath;
    std::string version;
    std::string application;
    bool initialized = false;

    void clear();

public:
    DeviceCatalog() = default;
    ~DeviceCatalog();

    bool init(const std::string &configFile = "");
    bool isInitialized() const { return initialized; }

    BaseDevice *getDevice(const std::string &uid) const;
    const std::vector<BaseDevice *> &getDevices() const { return devices; }
    const std::string &getVersion() const { return version; }
    const std::string &getApplication() const { return application; }
    const std::string &getConfigFilePath() const { return configFilePath; }
};

#endif // DEVICE_CATALOG_HPP
