/**
 * @file base_device.hpp
 * @brief Declaration and implementation of sensor classes for sensor management using built-in exceptions.
 *
 * This header defines the abstract BaseDevice class and its derived ADC and CustomSensor classes.
 * BaseDevice provides a helper method printBasicInfo() to output common sensor details.
 * Derived classes override printDevice() to print extra sensor-specific information.
 * Factory functions are provided to create sensor instances.
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
#include "../exceptions/devices_exceptions.hpp" ///< Sensor related exceptions.
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
 * @brief Enumeration representing possible sensor statuses.
 *
 * - OK: Sensor is operating normally.
 * - ERROR: Sensor has encountered an error.
 * - OFFLINE: Sensor is offline.
 */
enum class DeviceStatus
{
    OK = 1,     ///< Sensor operating normally.
    ERROR = -1, ///< Sensor has an error.
    OFFLINE = 0 ///< Sensor is offline.
};

/**
 * @enum DeviceCommandsEnum
 * @brief Enumeration representing possible sensor commands.
 *
 * - CONFIG: Configure sensor.
 * - UPDATE: Update sensor.
 * - INIT: Initialize sensor.
 * - RESET: Reset sensor.
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
 * @brief Structure for sensor parameter restrictions.
 *
 * This structure can be used to define restrictions for sensor parameters such as minimum,
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
 * @brief Structure for sensor parameters.
 *
 * This structure can be used to store sensor parameters for configuration and updating.
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
 * @brief Abstract base class for sensors.
 *
 * Defines common properties and virtual methods for sensor initialization, configuration,
 * updating, and printing. It also provides a helper method, printBasicInfo(), that prints
 * common sensor details.
 */
class BaseDevice
{
protected:
    bool redrawPending = true;  ///< Flag to indicate if sensor needs to be redrawn.
    bool isConfigsSync = false; ///< Flag to indicate if sensor congig is synchronized with real sensor.
    bool isValuesSync = false;  ///< Flag to indicate if sensor values is synchronized with real sensor.
    bool isControlsSync = true; ///< Flag to indicate if control payload is synchronized with real device.

    std::unordered_map<std::string, DeviceParam> Values;            ///< Sensor values.
    std::unordered_map<std::string, DeviceParam> Configs;          ///< Sensor configurations.
    std::vector<std::string> ValueKeyOrder;                        ///< Stable UI order for value keys.
    std::vector<std::string> ConfigKeyOrder;                       ///< Stable UI order for config keys.
    std::vector<std::string> Pins;                                 ///< Sensor pins.
    std::string AllowedPins;                                       ///< Allowed sensor pins, enter as list of values separated by ",".
    DeviceRole Role = DeviceRole::SENSOR;                          ///< Runtime device role.

    /**
     * @brief Set sensor status.
     *
     * This function sets the sensor status based on the given status string.
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
     * @brief Set sensor status.
     *
     * This function sets the sensor status based on the given status.
     *
     * @param status The status.
     */
    void setStatus(DeviceStatus status)
    {
        Status = status;
    }


    /**
     * @brief Synchronize sensor configurations with real sensor.
     *
     * This function sends a request to the real sensor to synchronize the configurations.
     */
    void syncConfigs()
    {
        isConfigsSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
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

        isConfigsSync = response.status == ResponseStatusEnum::OK; // Set flag to indicate sensor is synchronized with real sensor.
        redrawPending = isConfigsSync; // Set flag to redraw sensor - values updated.
    }

    /**
     * @brief Synchronize sensor values with real sensor.
     *
     * This function sends a request to the real sensor to synchronize the values.
     */
    void syncValues()
    {
        try
        {
            isValuesSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
            redrawPending = false; // Reset redraw flag.

            auto response = Protocol::update(UID);
            if (response.status == ResponseStatusEnum::ERROR)
            {
                throw DeviceSynchronizationFailException("BaseDevice::syncValues", response.error);
            }

            update(response.params); // Update sensor values from response parameters

            isValuesSync = response.status == ResponseStatusEnum::OK; // Set flag to indicate sensor is synchronized with real sensor.
            redrawPending = isValuesSync; // Set flag to redraw sensor - values updated.
        }
        catch (...)
        {
            throw;
        }
    }



    /**
     * @brief Check if the given value meets the restrictions defined in the sensor parameter.
     *
     * @param value The value to check.
     * @param param The sensor parameter containing the restrictions.
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
    std::string UID;         ///< Unique sensor identifier.
    DeviceStatus Status;     ///< Sensor status.
    std::string Type;        ///< Sensor type as text.
    std::string Description; ///< Description of the sensor.
    std::string LastError;   ///< Error message (if any).

    // lv_obj_t *ui_Container; ///< Pointer to the UI widgets container.
    /**
     * @brief Equality operator for comparing sensors by UID.
     *
     * @param sensor The sensor to compare with.
     * @return true if the sensors have the same UID, false otherwise.
     */
    bool operator==(const BaseDevice &sensor) const
    {
        return UID == sensor.UID;
    }

    /**
     * @brief Equality operator for comparing sensors by UID.
     *
     * @param uid The UID to compare with.
     * @return true if the sensor's UID matches the given UID, false otherwise.
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
     * @param uid The unique sensor identifier.
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
     * @brief Get the sensor unique identifier.
     *
     * @return The sensor UID.
     */
    std::string getId() const { return UID; }

    /**
     * @brief Get the sensor name (same as UID for compatibility).
     *
     * @return The sensor name.
     */
    std::string getName() const { return Type + " (" + UID + ")"; }

    /**
     * @brief Get the sensor type name.
     *
     * @return The sensor type.
     */
    std::string getTypeName() const { return Type; }

    /**
     * @brief Get the sensor description.
     *
     * @return The sensor description.
     */
    std::string getDescription() const { return Description; }

    /**
     * @brief Get the sensor status.
     *
     * @return The sensor status.
     */
    DeviceStatus getStatus() const { return Status; }

    /**
     * @brief Assign a pin to the sensor.
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
     * @brief Unassign a pin from the sensor.
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
     * @brief Check whether a pin number is allowed for this sensor.
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
     * @brief Connect the sensor to its assigned pins.
     * 
     */
    bool connect() 
    {
        std::string pins = getPins();
        if(pins.empty()) {
            throw DevicePinAssignmentException("connectDevice", "No pins assigned to sensor.");
        }

        auto response = Protocol::connect(UID, pins);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw DeviceConnectionFailException("BaseDevice::connect", response.error);
        }

        return response.status == ResponseStatusEnum::OK;
    }

    /**
     * @brief Disconnect the sensor from its assigned pins.
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

        isConfigsSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
        redrawPending = true;
    }

    /**
     * @brief Get value from sensor.
     *
     * This function retrieves the value of a sensor parameter by key.
     *
     * @param key The key of the sensor parameter.
     * @return The value of the sensor parameter.
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
     * @brief Set sensor value.
     *
     * This function sets the value of a sensor parameter by key.
     *
     * @param key The key of the sensor parameter.
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
     * @brief Get units of sensor value parameter.
     *
     * This function retrieves the units of a sensor value parameter by key.
     *
     * @param key The key of the value sensor parameter.
     * @return The units of the value sensor parameter.
     */
    std::string getValueUnits(const std::string &key)
    {
        if (Values.find(key) != Values.end())
        {
            return Values[key].Unit;
        }
        return "";
    }

    /**
     * @brief Get units of sensor config parameter.
     *
     * This function retrieves the units of a sensor config parameter by key.
     *
     * @param key The key of the sensor config parameter.
     * @return The units of the sensor config parameter.
     */
    std::string getConfigUnits(const std::string &key)
    {
        if (Configs.find(key) != Configs.end())
        {
            return Configs[key].Unit;
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
     * @brief Get history of sensor value parameter.
     *
     * This function retrieves the history of a sensor value parameter by key.
     *
     * @param key The key of the value sensor parameter.
     * @return The history array of the value sensor parameter.
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
     * @brief Clear history of all sensor value parameters.
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
     * @brief Synchronize with the real sensor.
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
     * @brief Add configuration parameter to the sensor.
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

        isConfigsSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
    }

    /**
     * @brief Configures the sensor with the given configuration map.
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
            // Parse the config string and update the sensor configs.
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

                    redrawPending = true; // Set flag to redraw sensor - values updated.
                }
            }
        }
        catch(...)
        {
            throw;
        }
    }

    /**
     * @brief Adds a value parameter to the sensor.
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

        isValuesSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
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
     * @brief Updates the sensor with new data.
     *
     * @param upd The update map containing new sensor data.
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
            // Parse the update string and update the sensor values.
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

                    redrawPending = true; // Set flag to redraw sensor - values updated.
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
     * @brief Prints sensor information.
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
     * @brief Initializes the sensor.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init()
    {
        redrawPending = true; // Set flag to redraw sensor - values updated.
        isConfigsSync = true; // Set flag to indicate sensor is synchronized by default with real sensor.
        isValuesSync = false; // Set flag to indicate sensor is not synchronized with real sensor
        isControlsSync = true; // Control payload is synchronized until the user changes it locally.

        clearError();
    };
};

/**************************************************************************/
// CREATE FUNCTIONS
/**************************************************************************/

/**
 * @brief Factory function template to create a sensor of type T.
 *
 * This function creates a sensor object of type T (which must have a constructor taking an int)
 * and returns a pointer to the newly created object. If initialization fails, it logs the error,
 * deletes the partially constructed object, and rethrows the exception.
 *
 * @tparam T The sensor type, which must be derived from BaseDevice.
 * @param uid The unique sensor identifier.
 * @return T* Pointer to the newly created sensor.
 * @throws DeviceInitializationFailException if sensor initialization fails.
 */
template <typename T>
T *createDevice(std::string uid)
{
    static_assert(std::is_base_of<BaseDevice, T>::value, "T must be derived from BaseDevice");

    T *sensor = nullptr;
    try
    {
        sensor = new T(uid);
    }
    catch (const std::exception &ex)
    {
        logMessage("Error during sensor initialization: %s\n", ex.what());
        delete sensor;
        throw DeviceInitializationFailException("createDevice", "Error during device initialization.", new Exception(ex));
    }

    logMessage("Device [%s]:%s created successfully.\n", sensor->UID.c_str(), sensor->Type.c_str());
    return sensor;
}

/**************************************************************************/
// GENERAL FUNCTIONS
/**************************************************************************/

/**
 * @brief Configures the sensor using the given configuration string.
 *
 * This function applies the configuration specified in the string to the provided sensor.
 * The configuration string should follow the expected format for that sensor (e.g. "Resolution=12").
 *
 * @param sensor Pointer to the sensor to be configured.
 * @param config The configuration string.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool configDevice(BaseDevice *device, const std::string &config);

/**
 * @brief Updates the sensor with new measurement data.
 *
 * This function updates the sensor's parameters based on the provided update string.
 * The update string should follow the expected format (e.g. "Value=3.3").
 *
 * @param sensor Pointer to the sensor to be updated.
 * @param update The update string containing new sensor data.
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
 * @brief Prints detailed information about the sensor.
 *
 * This function prints sensor details by calling the sensor's own printDevice() method,
 * which includes both basic sensor info and any additional sensor-specific data.
 *
 * @param sensor Pointer to the sensor whose information is to be printed.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
void printDevice(BaseDevice *device);

/**
 * @brief Synchronizes the sensor with the real sensor.
 *
 * This function synchronizes the sensor with the real sensor by calling the sensor's synchronize() method.
 *
 * @param sensor Pointer to the sensor to be synchronized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool syncDevice(BaseDevice *device);

/**
 * @brief Initializes the sensor.
 *
 * This function initializes the sensor by calling the sensor's init() method.
 *
 * @param sensor Pointer to the sensor to be initialized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool initDevice(BaseDevice *device);

/**
 * @brief Connect the sensor to the specified pins.
 * 
 * @param sensor Pointer to the sensor to be connected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool connectDevice(BaseDevice *device);

/**
 * @brief Disconnect the sensor from its current pins.
 * 
 * @param sensor Pointer to the sensor to be disconnected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool disconnectDevice(BaseDevice *device);

#endif // BASE_DEVICE_HPP
