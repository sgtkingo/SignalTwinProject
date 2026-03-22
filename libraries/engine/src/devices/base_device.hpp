/**
 * @file base_device.hpp
 * @brief Declaration and implementation of the runtime device model and helpers.
 *
 * This header defines the BaseDevice runtime type together with shared helper
 * functions for configuration, synchronization, connection and diagnostics.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 *
 */

#ifndef BASE_DEVICE_HPP
#define BASE_DEVICE_HPP

/*********************
 *      INCLUDES
 *********************/
#include "vscp.hpp"
#include "../exceptions/devices_exceptions.hpp" ///< Device related exceptions.
#include "../helpers.hpp"    ///< Helper functions.

#include <string>
#include <unordered_map>
#include <map>
#include <array>
#include <cstddef>
#include <vector>

#define HISTORY_CAP 10 ///< History capacity.

/**
 * @enum DeviceStatus
 * @brief Enumeration representing possible device statuses.
 *
 * - OK: Device is operating normally.
 * - ERROR: Device has encountered an error.
 * - OFFLINE: Device is offline.
 */
enum class DeviceStatus
{
    OK = 1,     ///< Device operating normally.
    ERROR = -1, ///< Device has an error.
    OFFLINE = 0 ///< Device is offline.
};

/**
 * @enum DeviceCommandsEnum
 * @brief Enumeration representing possible device protocol commands.
 *
 * - CONFIG: Configure the device.
 * - UPDATE: Read runtime values from the device.
 * - CONTROL: Send runtime control payload to the device.
 * - INIT: Initialize the device.
 * - RESET: Reset the device.
 */
enum class DeviceCommandsEnum
{
    CONFIG,
    UPDATE,
    CONTROL,
    INIT,
    RESET,
    CONNECT,
    DISCONNECT
};

/**
 * @enum enum class DeviceDataType
 * @brief Enumeration representing possible parametrs data types.
 *
 * - INT: int.
 * - DOUBLE: double.
 *  - FLOAT: float.
 * - STRING: string.
 */
enum class DeviceDataType
{
    INT,
    DOUBLE,
    FLOAT,
    STRING
};

/**
 * @enum DeviceRole
 * @brief Declares whether the runtime device behaves as a sensor, actuator, or both.
 */
enum class DeviceRole
{
    SENSOR,
    ACTUATOR,
    HYBRID
};


/**
 * @struct DeviceRestrictions
 * @brief Structure for device parameter restrictions.
 *
 * This structure can be used to define restrictions for device parameters such as minimum,
 * maximum, step size, and options (for enum types).
 */
struct DeviceRestrictions
{
    std::string Min;
    std::string Max;
    std::string Step;
    std::string Options; ///< Comma separated list of options (for enum types).
};

/**
 * @struct DeviceParam
 * @brief Structure for device parameters.
 *
 * This structure can be used to store runtime values and configuration parameters.
 */
struct DeviceParam
{
    std::string Value;                ///< Parameter value.
    std::string Unit;                 ///< Parameter unit.
    DeviceDataType DType;                   ///< Parameter data type.
    int lastHistoryIndex;             ///< Last history index.
    std::string History[HISTORY_CAP]; ///< Parameter history.
    DeviceRestrictions Restrictions;  ///< Parameter restrictions.
};

/**
 * @class BaseDevice
 * @brief Abstract base class for devices.
 *
 * Defines common properties and virtual methods for device initialization,
 * configuration, synchronization, control, and diagnostics.
 */
class BaseDevice
{
protected:
    bool redrawPending = true;  ///< Flag to indicate if the device needs to be redrawn.
    bool isConfigsSync = false; ///< Flag to indicate if config state is synchronized with the real device.
    bool isValuesSync = false;  ///< Flag to indicate if runtime values are synchronized with the real device.
    bool isControlsSync = true; ///< Flag to indicate if control payload is synchronized with real device.

    std::unordered_map<std::string, DeviceParam> Values;            ///< Runtime values.
    std::unordered_map<std::string, DeviceParam> Configs;           ///< Persistent configuration values.
    std::vector<std::string> ValueKeyOrder;                        ///< Stable UI order for value keys.
    std::vector<std::string> ConfigKeyOrder;                       ///< Stable UI order for config keys.
    std::vector<std::string> Pins;                                 ///< Assigned device pins.
    std::string AllowedPins;                                       ///< Allowed device pins, stored as comma separated values.
    DeviceRole Role = DeviceRole::SENSOR;                          ///< Runtime device role.

    /**
     * @brief Set device status from protocol string.
     *
     * This function sets the device status based on the given status string.
     *
     * @param status The status string.
     */
    void setStatus(std::string status)
    {
        if (status.empty())
        {
            return;
        }

        if (status == "1")
        {
            Status = DeviceStatus::OK;
        }
        else if (status == "-1")
        {
            Status = DeviceStatus::ERROR;
        }
        else if (status == "0")
        {
            Status = DeviceStatus::OFFLINE;
        }
    }

    /**
     * @brief Set device status.
     *
     * This function sets the device status based on the given status.
     *
     * @param status The status.
     */
    void setStatus(DeviceStatus status)
    {
        Status = status;
    }


    /**
     * @brief Synchronize configuration values with the real device.
     *
     * This function sends a request to the real device to synchronize configuration values.
     */
    void syncConfigs()
    {
        isConfigsSync = false; // Set flag to indicate config state is not synchronized with the real device.
        redrawPending = false; // Reset redraw flag.

        //Convert Configs to unordered_map<std::string, std::string>
        std::unordered_map<std::string, std::string> configMap;
        for (const auto &pair : Configs)
        {
            configMap[pair.first] = pair.second.Value;
        }
        auto response = Protocol::config(UID, configMap);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw DeviceSynchronizationFailException("BaseDevice::syncConfigs", response.error);
        }

        isConfigsSync = response.status == ResponseStatusEnum::OK; // Set flag to indicate config state is synchronized with the real device.
        redrawPending = isConfigsSync; // Redraw after config changes are acknowledged.
    }

    /**
     * @brief Synchronize runtime values with the real device.
     *
     * This function sends a request to the real device to read runtime values.
     */
    void syncValues()
    {
        try
        {
            isValuesSync = false; // Set flag to indicate runtime values are not synchronized with the real device.
            redrawPending = false; // Reset redraw flag.

            auto response = Protocol::update(UID);
            if (response.status == ResponseStatusEnum::ERROR)
            {
                throw DeviceSynchronizationFailException("BaseDevice::syncValues", response.error);
            }

            update(response.params); // Update runtime values from response parameters.

            isValuesSync = response.status == ResponseStatusEnum::OK; // Set flag to indicate runtime values are synchronized with the real device.
            redrawPending = isValuesSync; // Redraw after runtime values are updated.
        }
        catch (...)
        {
            throw;
        }
    }



    /**
     * @brief Check if the given value meets the restrictions defined in the device parameter.
     *
     * @param value The value to check.
     * @param param The device parameter containing the restrictions.
     * @return true if the value meets the restrictions, false otherwise.
     */
    bool checkRestrictions(std::string value, const DeviceParam &param)
    {
        DeviceRestrictions restrictions = param.Restrictions;
        try
        {
            if (!restrictions.Min.empty())
            {
                double min = convertStringToType<double>(restrictions.Min);
                double val = convertStringToType<double>(value);
                if (val < min)
                {
                    return false;
                }
            }

            if (!restrictions.Max.empty())
            {
                double max = convertStringToType<double>(restrictions.Max);
                double val = convertStringToType<double>(value);
                if (val > max)
                {
                    return false;
                }
            }

            if (!restrictions.Options.empty())
            {
                std::vector<std::string> options = splitString(restrictions.Options, ',');
                if (std::find(options.begin(), options.end(), value) == options.end())
                {
                    return false;
                }
            }
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("BaseDevice::checkRestrictions", e.what());
        }

        return true;
    }

public:
    std::string UID;         ///< Unique device identifier.
    DeviceStatus Status;     ///< Device status.
    std::string Type;        ///< Device type as text.
    std::string Description; ///< Human readable device description.
    std::string LastError;   ///< Error message (if any).

    // lv_obj_t *ui_Container; ///< Pointer to the UI widgets container.
    /**
     * @brief Equality operator for comparing devices by UID.
     *
     * @param device The device to compare with.
     * @return true if the devices have the same UID, false otherwise.
     */
    bool operator==(const BaseDevice &device) const
    {
        return UID == device.UID;
    }

    /**
     * @brief Equality operator for comparing devices by UID.
     *
     * @param uid The UID to compare with.
     * @return true if the device UID matches the given UID, false otherwise.
     */
    bool operator==(const std::string uid) const
    {
        return UID == uid;
    }

    /**
     * @brief Constructs a new BaseDevice object.
     */
    BaseDevice() : UID("DummyDevice"), Status(DeviceStatus::OK)
    {
        LastError = "";

        init();
    }

    /**
     * @brief Constructs a new BaseDevice object.
     *
     * @param uid The unique device identifier.
     */
    BaseDevice(std::string uid) : UID(uid), Status(DeviceStatus::OK)
    {
        LastError = "";

        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseDevice()
    {
    }

    std::unordered_map<std::string, DeviceParam> getValues() const { return Values; }
    std::vector<std::string> getValuesKeys() const
    {
        if (!ValueKeyOrder.empty()) {
            return ValueKeyOrder;
        }

        std::vector<std::string> keys;
        for (const auto &pair : Values)
        {
            keys.push_back(pair.first);
        }
        return keys;
    }
    std::unordered_map<std::string, DeviceParam> getConfigs() const { return Configs; }
    std::vector<std::string> getConfigsKeys() const
    {
        if (!ConfigKeyOrder.empty()) {
            return ConfigKeyOrder;
        }

        std::vector<std::string> keys;
        for (const auto &pair : Configs)
        {
            keys.push_back(pair.first);
        }
        return keys;
    }

    /**
     * @brief Get the redraw pending status.
     *
     * @return true if redraw is pending, false otherwise.
     */
    bool getRedrawPending() const { return redrawPending; }

    /**
     * @brief Get the runtime role of the device.
     *
     * @return DeviceRole The role of the current device.
     */
    DeviceRole getRole() const { return Role; }

    /**
     * @brief Set the runtime role of the device.
     *
     * @param role New device role.
     */
    void setRole(DeviceRole role) { Role = role; }

    /**
     * @brief Get a user-friendly role label.
     *
     * @return std::string Human readable role.
     */
    std::string getRoleLabel() const
    {
        switch (Role)
        {
        case DeviceRole::ACTUATOR:
            return "Actuator";
        case DeviceRole::HYBRID:
            return "Hybrid";
        case DeviceRole::SENSOR:
        default:
            return "Sensor";
        }
    }

    /**
     * @brief Synchronize runtime control payload with the real device.
     */
    void syncControls()
    {
        isControlsSync = false;
        redrawPending = false;

        std::unordered_map<std::string, std::string> valueMap;
        for (const auto &pair : Values)
        {
            valueMap[pair.first] = pair.second.Value;
        }

        auto response = Protocol::control(UID, valueMap);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw DeviceSynchronizationFailException("BaseDevice::syncControls", response.error);
        }

        isControlsSync = response.status == ResponseStatusEnum::OK;
        redrawPending = isControlsSync;
    }

    /**
     * @brief Check whether the device exposes live values.
     *
     * @return true when at least one value exists.
     */
    bool hasValues() const { return !Values.empty(); }

    /**
     * @brief Check whether the device exposes configurable parameters.
     *
     * @return true when at least one config exists.
     */
    bool hasConfigs() const { return !Configs.empty(); }

    /**
     * @brief Check whether the device should use CONFIG channel for synchronization.
     *
     * @return true if CONFIG sync is meaningful for this device.
     */
    bool usesConfigChannel() const
    {
        return hasConfigs();
    }

    /**
     * @brief Check whether the device should use UPDATE channel for synchronization.
     *
     * @return true if UPDATE sync is meaningful for this device.
     */
    bool usesUpdateChannel() const
    {
        return hasValues() && Role != DeviceRole::ACTUATOR;
    }

    /**
     * @brief Check whether the device should use CONTROL channel for synchronization.
     *
     * @return true if CONTROL sync is meaningful for this device.
     */
    bool usesControlChannel() const
    {
        return hasValues() && Role != DeviceRole::SENSOR;
    }

    /**
     * @brief Set the redraw pending status.
     *
     * @param pending The new redraw pending status.
     */
    void setRedrawPending(bool pending) { redrawPending = pending; }

    /**
     * @brief Get the device unique identifier.
     *
     * @return The device UID.
     */
    std::string getId() const { return UID; }

    /**
     * @brief Get the device name (same as UID for compatibility).
     *
     * @return The device name.
     */
    std::string getName() const { return Type + " (" + UID + ")"; }

    /**
     * @brief Get the device type name.
     *
     * @return The device type.
     */
    std::string getTypeName() const { return Type; }

    /**
     * @brief Get the device description.
     *
     * @return The device description.
     */
    std::string getDescription() const { return Description; }

    /**
     * @brief Get the device status.
     *
     * @return The device status.
     */
    DeviceStatus getStatus() const { return Status; }

    /**
     * @brief Assign a pin to the device.
     * 
     * @param pin The pin to assign.
     */
    void assignPin(std::string pin) 
    {
        if (isInVector(Pins, pin)) {
            return; // Pin already assigned
        }
        Pins.push_back(pin);
    }

    /**
     * @brief Unassign a pin from the device.
     * 
     * @param pin The pin to unassign.
     */
    void unassignPin(std::string pin) 
    {
        auto it = std::find(Pins.begin(), Pins.end(), pin);
        if (it != Pins.end()) {
            Pins.erase(it);
        }
    }

    /**
     * @brief Get the Pins object
     * 
     * @return std::string 
     */
    std::string getPins() const 
    {
        std::string pins;
        for (const auto &pin : Pins)
        {
            if (!pins.empty())
            {
                pins += ",";
            }
            pins += pin;
        }
        return pins;
    }

    /**
     * @brief Get allowed pins as a list of integers.
     *
     * Empty AllowedPins means all pins are allowed.
     *
     * @return Vector of allowed pin numbers
     */
    std::vector<int> getAllowedPinsList() const
    {
        std::vector<int> pins;
        if (AllowedPins.empty()) {
            return pins;
        }

        for (const auto &item : splitString(AllowedPins, ',')) {
            if (item.empty()) {
                continue;
            }
            pins.push_back(convertStringToType<int>(item));
        }
        return pins;
    }

    /**
     * @brief Check whether a pin number is allowed for this device.
     *
     * @param pinNumber The pin number to check
     * @return True if the pin is allowed or if the allowed list is empty
     */
    bool isPinAllowed(int pinNumber) const
    {
        if (AllowedPins.empty()) {
            return true;
        }

        const auto allowed = getAllowedPinsList();
        return std::find(allowed.begin(), allowed.end(), pinNumber) != allowed.end();
    }

    /**
     * @brief Connect the device to its assigned pins.
     * 
     */
    bool connect() 
    {
        std::string pins = getPins();
        if(pins.empty()) {
            throw DevicePinAssignmentException("connectDevice", "No pins assigned to device.");
        }

        auto response = Protocol::connect(UID, pins);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw DeviceConnectionFailException("BaseDevice::connect", response.error);
        }

        return response.status == ResponseStatusEnum::OK;
    }

    /**
     * @brief Disconnect the device from its assigned pins.
     * 
     */
    bool disconnect() 
    {
        auto response = Protocol::disconnect(UID);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw DeviceConnectionFailException("BaseDevice::disconnect", response.error);
        }

        if (response.status == ResponseStatusEnum::OK) {
            Pins.clear();
        }

        return response.status == ResponseStatusEnum::OK;
    }

    /**
     * @brief Get value from configuration.
     *
     * This function retrieves the value of a configuration parameter by key.
     *
     * @param key The key of the configuration parameter.
     * @return The value of the configuration parameter.
     */
    template <typename T>
    T getConfig(const std::string &key)
    {
        std::string value;
        if (Configs.find(key) != Configs.end())
        {
            value = Configs[key].Value;
        }
        if (value.empty())
        {
            throw ConfigurationNotFoundException("BaseDevice::getConfig", "Configuration not found for key: " + key);
        }

        try
        {
            return convertStringToType<T>(value);
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("BaseDevice::getConfig", e.what());
        }
    }

    /**
     * @brief Set configuration value.
     *
     * This function sets the value of a configuration parameter by key.
     *
     * @param key The key of the configuration parameter.
     * @param value The value to set.
     */
    void setConfig(const std::string &key, const std::string &value)
    {
        if (Configs.find(key) != Configs.end())
        {
            if (!checkRestrictions(value, Configs[key]))
            {
                throw InvalidValueException("BaseDevice::setConfig", "Value " + value + " for key " + key + " does not meet restrictions.");
            }
            Configs[key].Value = value;
        }
        else
        {
            throw ConfigurationNotFoundException("BaseDevice::setConfig", "Configuration not found for key: " + key);
        }

        isConfigsSync = false; // Set flag to indicate config state is not synchronized with the real device.
        redrawPending = true;
    }

    /**
     * @brief Get a runtime value from the device.
     *
     * This function retrieves the value of a device runtime parameter by key.
     *
     * @param key The key of the device value parameter.
     * @return The value of the device parameter.
     */
    template <typename T>
    T getValue(const std::string &key)
    {
        std::string value;
        if (Values.find(key) != Values.end())
        {
            value = Values[key].Value;
        }
        if (value.empty())
        {
            throw ValueNotFoundException("BaseDevice::getValue", "Value not found for key: " + key);
        }

        try
        {
            return convertStringToType<T>(value);
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("BaseDevice::getValue", e.what());
        }
    }

    /**
     * @brief Set a runtime value.
     *
     * This function sets the value of a device runtime parameter by key.
     *
     * @param key The key of the device value parameter.
     * @param value The value to set.
     */
    void setValue(const std::string &key, const std::string &value)
    {
        if (Values.find(key) != Values.end())
        {
            if (!checkRestrictions(value, Values[key]))
            {
                throw InvalidValueException("BaseDevice::setValue", "Value " + value + " for key " + key + " does not meet restrictions.");
            }
            Values[key].Value = value;
            Values[key].History[Values[key].lastHistoryIndex++] = value;
            if (Values[key].lastHistoryIndex >= HISTORY_CAP)
            {
                Values[key].lastHistoryIndex = 0;
            }
        }
        else
        {
            throw ValueNotFoundException("BaseDevice::setValue", "Value not found for key: " + key);
        }

        if (usesControlChannel())
        {
            isControlsSync = false;
        }

        redrawPending = true;
    }

    /**
     * @brief Get units of a device value parameter.
     *
     * This function retrieves the units of a device value parameter by key.
     *
     * @param key The key of the device value parameter.
     * @return The units of the device value parameter.
     */
    std::string getValueUnits(const std::string &key) const
    {
        auto it = Values.find(key);
        if (it != Values.end())
        {
            return it->second.Unit;
        }
        return "";
    }

    /**
     * @brief Get units of a device config parameter.
     *
     * This function retrieves the units of a device config parameter by key.
     *
     * @param key The key of the device config parameter.
     * @return The units of the device config parameter.
     */
    std::string getConfigUnits(const std::string &key) const
    {
        auto it = Configs.find(key);
        if (it != Configs.end())
        {
            return it->second.Unit;
        }
        return "";
    }

    /**
     * @brief Set exception as error and change status accordingly.
     *
     * @param error The exception as error.
     */
    void setError(std::string error)
    { 
        LastError = error;
        if (!error.empty())
        {
            setStatus(DeviceStatus::ERROR);
        }
        else
        {
            setStatus(DeviceStatus::OK);
        }
    }

    /**
     * @brief Get error message.
     *
     * @return The error message.
     */
    std::string getError() const
    {
        return LastError;
    }

    void clearError() 
    {
        setError("");
    }

    /**
     * @brief Get history of a device value parameter.
     *
     * This function retrieves the history of a device value parameter by key.
     *
     * @param key The key of the device value parameter.
     * @return The history array of the device value parameter.
     */
    std::string* getHistory(const std::string &key)
    {
        //Find key in Values and return history array
        if (Values.find(key) != Values.end())
        {
            // Return the history array
            return Values[key].History;
        }

        throw ValueNotFoundException("BaseDevice::getHistory", "Value not found for key: " + key);
    }

    /**
     * @brief Clear history of all device value parameters.
     */
    void clearHistory()
    {
        for (auto &v : Values)
        {
            for(int i=0;i<HISTORY_CAP;i++){
                v.second.History[i] = "0";
                //logMessage("Clearing history value %s for key %s\n", v.second.History[i], v.first.c_str());
            }
            v.second.lastHistoryIndex = 0;
        }
    }

    /**
     * @brief Synchronize with the real device.
     *
     * @throws Exception if synchronization fails.
     */
    virtual bool synchronize()
    {
        const bool syncConfigsChannel = usesConfigChannel();
        const bool syncValuesChannel = usesUpdateChannel();
        const bool syncControlChannel = usesControlChannel();

        if (!syncConfigsChannel)
        {
            isConfigsSync = true;
        }

        if (!syncValuesChannel)
        {
            isValuesSync = true;
        }

        if (!syncControlChannel)
        {
            isControlsSync = true;
        }

        if (syncConfigsChannel && !isConfigsSync)
        {
            try
            {
                syncConfigs();
            }
            catch (...)
            {
                throw;
            }
        }

        if (syncControlChannel && !isControlsSync)
        {
            try
            {
                syncControls();
            }
            catch (...)
            {
                throw;
            }
        }

        if (syncValuesChannel && !isValuesSync)
        {
            try
            {
                syncValues();
            }
            catch (...)
            {
                throw;
            }
        }

        return (!syncValuesChannel || isValuesSync) &&
               (!syncConfigsChannel || isConfigsSync) &&
               (!syncControlChannel || isControlsSync);
    }

    /**
     * @brief Add a configuration parameter to the device.
     *
     * @param key The key of the configuration parameter.
     * @param param The configuration parameter to add.
     */
    void addConfigParameter(const std::string &key, const DeviceParam &param)
    {
        try
        {
            Configs[key] = param;
            if (!isInVector(ConfigKeyOrder, key)) {
                ConfigKeyOrder.push_back(key);
            }
        }
        catch (const std::exception &e)
        {
            throw InvalidConfigurationException("BaseDevice::addConfigParameter", e.what());
        }

        isConfigsSync = false; // Set flag to indicate config state is not synchronized with the real device.
    }

    /**
     * @brief Configure the device with the given configuration map.
     *
     * @param cfg The configuration map.
     * @throws Exception if configuration fails.
     */
    virtual void config(const std::unordered_map<std::string, std::string> &cfg)
    {
        if(cfg.empty())
        {
            return;
        }
        std::string value;
        try
        {
            // Parse the config string and update the device configs.
            for (auto &c : Configs)
            {
                value = cfg.find(c.first) != cfg.end() ? cfg.at(c.first) : "";
                if (!value.empty())
                {
                    if (!checkRestrictions(value, c.second))
                    {
                        throw InvalidValueException("BaseDevice::config", "Value " + value + " for key " + c.first + " does not meet restrictions.");
                    }
                    c.second.Value = value;

                    c.second.History[c.second.lastHistoryIndex++] = value;
                    if (c.second.lastHistoryIndex >= HISTORY_CAP)
                    {
                        c.second.lastHistoryIndex = 0;
                    }

                    redrawPending = true; // Redraw after configuration values are updated.
                }
            }
        }
        catch(...)
        {
            throw;
        }
    }

    /**
     * @brief Add a value parameter to the device.
     *
     * @param key The key of the value parameter.
     * @param param The value parameter to add.
     * @throws Exception if adding the value parameter fails.
     */
    void addValueParameter(const std::string &key, const DeviceParam &param)
    {
        try
        {
            Values[key] = param;
            if (!isInVector(ValueKeyOrder, key)) {
                ValueKeyOrder.push_back(key);
            }
        }
        catch (const std::exception &e)
        {
            throw InvalidValueException("BaseDevice::addValueParameter", new Exception(e));
        }

        isValuesSync = false; // Set flag to indicate runtime values are not synchronized with the real device.
    }

    /**
     * @brief Applies runtime control payload to device values.
     *
     * @param ctl The control map containing new device values.
     */
    virtual void control(const std::unordered_map<std::string, std::string> &ctl)
    {
        update(ctl);
    }

    /**
     * @brief Update the device with new runtime data.
     *
     * @param upd The update map containing new device data.
     * @throws Exception if update fails.
     */
    virtual void update(const std::unordered_map<std::string, std::string> &upd)
    {
        if(upd.empty())
        {
            return;
        }

        std::string value;
        try
        {
            // Parse the update string and update the device values.
            for (auto &c : Values)
            {
                value = upd.find(c.first) != upd.end() ? upd.at(c.first) : "";
                if (!value.empty())
                {
                    if (!checkRestrictions(value, c.second))
                    {
                        throw InvalidValueException("BaseDevice::update", "Value " + value + " for key " + c.first + " does not meet restrictions.");
                    }
                    c.second.Value = value;
                    c.second.History[c.second.lastHistoryIndex++] = value;
                    if (c.second.lastHistoryIndex >= HISTORY_CAP)
                    {
                        c.second.lastHistoryIndex = 0;
                    }

                    redrawPending = true; // Redraw after runtime values are updated.
                }
            }

            std::string status = upd.find("status") != upd.end() ? upd.at("status") : "-1";
        }
        catch(...)
        {
            throw;
        }
    }

    /**
     * @brief Print device information.
     */
    void print() const
    {
        logMessage("Device UID: %s\n", UID.c_str());
        logMessage("\tDevice Type: %s\n", Type.c_str());
        logMessage("\tDevice Description: %s\n", Description.c_str());
        logMessage("\tDevice Status: %d\n", Status);
        logMessage("\tDevice Error: %s\n", getError().c_str());
        logMessage("\tDevice Configurations:\n");
        for (auto &c : Configs)
        {
            logMessage("\t\t%s: %s %s\n", c.first.c_str(), c.second.Value.c_str(), c.second.Unit.c_str());
        }
        logMessage("\tDevice Values:\n");
        for (auto &v : Values)
        {
            logMessage("\t\t%s: %s %s\n", v.first.c_str(), v.second.Value.c_str(), v.second.Unit.c_str());
        }
        logMessage("\tDevice Pins: %s\n", getPins().c_str());
        logMessage("**************************************\n");
    }

    /**
     * @brief Initialize the device.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init()
    {
        redrawPending = true; // Redraw after initialization.
        isConfigsSync = true; // Config state starts synchronized by default.
        isValuesSync = false; // Runtime values must be fetched from the real device.
        isControlsSync = true; // Control payload is synchronized until the user changes it locally.

        clearError();
    };
};

/**************************************************************************/
// CREATE FUNCTIONS
/**************************************************************************/

/**
 * @brief Factory function template to create a device of type T.
 *
 * This function creates a device object of type T (which must have a constructor taking an int)
 * and returns a pointer to the newly created object. If initialization fails, it logs the error,
 * deletes the partially constructed object, and rethrows the exception.
 *
 * @tparam T The device type, which must be derived from BaseDevice.
 * @param uid The unique device identifier.
 * @return T* Pointer to the newly created device.
 * @throws DeviceInitializationFailException if device initialization fails.
 */
template <typename T>
T *createDevice(std::string uid)
{
    static_assert(std::is_base_of<BaseDevice, T>::value, "T must be derived from BaseDevice");

    T *device = nullptr;
    try
    {
        device = new T(uid);
    }
    catch (const std::exception &ex)
    {
        logMessage("Error during device initialization: %s\n", ex.what());
        delete device;
        throw DeviceInitializationFailException("createDevice", "Error during device initialization.", new Exception(ex));
    }

    logMessage("Device [%s]:%s created successfully.\n", device->UID.c_str(), device->Type.c_str());
    return device;
}

/**************************************************************************/
// GENERAL FUNCTIONS
/**************************************************************************/

/**
 * @brief Configure the device using the given configuration string.
 *
 * This function applies the configuration specified in the string to the provided device.
 * The configuration string should follow the expected device-specific format (e.g. "Resolution=12").
 *
 * @param device Pointer to the device to be configured.
 * @param config The configuration string.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool configDevice(BaseDevice *device, const std::string &config);

/**
 * @brief Update the device with new runtime data.
 *
 * This function updates the device parameters based on the provided update string.
 * The update string should follow the expected format (e.g. "Value=3.3").
 *
 * @param device Pointer to the device to be updated.
 * @param update The update string containing new device data.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool updateDevice(BaseDevice *device, const std::string &update);

/**
 * @brief Applies control payload to the device.
 *
 * @param device Pointer to the device to control.
 * @param control The control string containing runtime output values.
 * @return true if the payload was accepted locally.
 */
bool controlDevice(BaseDevice *device, const std::string &control);

/**
 * @brief Print detailed information about the device.
 *
 * This function prints device details by calling the device print method,
 * which includes both basic info and any additional device-specific data.
 *
 * @param device Pointer to the device whose information is to be printed.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
void printDevice(BaseDevice *device);

/**
 * @brief Synchronize the device with the real device.
 *
 * This function synchronizes the device with the real hardware by calling the device synchronize() method.
 *
 * @param device Pointer to the device to be synchronized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool syncDevice(BaseDevice *device);

/**
 * @brief Initialize the device.
 *
 * This function initializes the device by calling the device init() method.
 *
 * @param device Pointer to the device to be initialized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool initDevice(BaseDevice *device);

/**
 * @brief Connect the device to the specified pins.
 * 
 * @param device Pointer to the device to be connected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool connectDevice(BaseDevice *device);

/**
 * @brief Disconnect the device from its current pins.
 * 
 * @param device Pointer to the device to be disconnected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool disconnectDevice(BaseDevice *device);

#endif // BASE_DEVICE_HPP
