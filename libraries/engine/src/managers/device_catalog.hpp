#ifndef DEVICE_CATALOG_HPP
#define DEVICE_CATALOG_HPP

#include <string>
#include <vector>

#include "../devices/base_device.hpp"
#include "../devices/json_device_builder.hpp"

/**
 * @brief Owns the boot-time device catalog loaded from JSON on the configured storage backend.
 *
 * The catalog stores both the runtime devices used by the application and
 * the editable schema used by Library persistence.
 */
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

    /**
     * @brief Load the catalog schema from storage and build runtime devices.
     */
    bool init(const std::string &configFile = "");

    /**
     * @brief Upsert a single Library draft into the schema and rebuild runtime devices.
     */
    bool saveDraft(const DeviceDefinitionSchema &draft, const std::string &originalUid, bool isNewEntity);

    /**
     * @brief Save top-level catalog metadata such as application and version.
     */
    bool saveMetadata(const std::string &applicationValue, const std::string &versionValue);

    /**
     * @brief Remove a device definition from the schema and rebuild runtime devices.
     */
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
