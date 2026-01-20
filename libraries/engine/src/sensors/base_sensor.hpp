/**
 * @file base_sensor.hpp
 * @brief Declaration and implementation of sensor classes for sensor management using built-in exceptions.
 *
 * This header defines the abstract BaseSensor class and its derived ADC and CustomSensor classes.
 * BaseSensor provides a helper method printBasicInfo() to output common sensor details.
 * Derived classes override printSensor() to print extra sensor-specific information.
 * Factory functions are provided to create sensor instances.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 *
 */

#ifndef BASE_SENSOR_HPP
#define BASE_SENSOR_HPP

/*********************
 *      INCLUDES
 *********************/
#include "vscp.hpp"
#include "../exceptions/sensors_exceptions.hpp" ///< Sensor related exceptions.
#include "../helpers.hpp"    ///< Helper functions.

#include <string>
#include <unordered_map>
#include <map>
#include <array>
#include <cstddef>

#define HISTORY_CAP 10 ///< History capacity.

/**
 * @enum SensorStatus
 * @brief Enumeration representing possible sensor statuses.
 *
 * - OK: Sensor is operating normally.
 * - ERROR: Sensor has encountered an error.
 * - OFFLINE: Sensor is offline.
 */
enum class SensorStatus
{
    OK = 1,     ///< Sensor operating normally.
    ERROR = -1, ///< Sensor has an error.
    OFFLINE = 0 ///< Sensor is offline.
};

/**
 * @enum SensorsCommandsEnum
 * @brief Enumeration representing possible sensor commands.
 *
 * - CONFIG: Configure sensor.
 * - UPDATE: Update sensor.
 * - INIT: Initialize sensor.
 * - RESET: Reset sensor.
 */
enum class SensorsCommandsEnum
{
    CONFIG,
    UPDATE,
    INIT,
    RESET,
    CONNECT,
    DISCONNECT
};

/**
 * @enum enum class SensorDataType
 * @brief Enumeration representing possible parametrs data types.
 *
 * - INT: int.
 * - DOUBLE: double.
 *  - FLOAT: float.
 * - STRING: string.
 */
enum class SensorDataType
{
    INT,
    DOUBLE,
    FLOAT,
    STRING
};


/**
 * @struct SensorRestrictions
 * @brief Structure for sensor parameter restrictions.
 *
 * This structure can be used to define restrictions for sensor parameters such as minimum,
 * maximum, step size, and options (for enum types).
 */
struct SensorRestrictions
{
    std::string Min;
    std::string Max;
    std::string Step;
    std::string Options; ///< Comma separated list of options (for enum types).
};

/**
 * @struct SensorParam
 * @brief Structure for sensor parameters.
 *
 * This structure can be used to store sensor parameters for configuration and updating.
 */
struct SensorParam
{
    std::string Value;                ///< Parameter value.
    std::string Unit;                 ///< Parameter unit.
    SensorDataType DType;                   ///< Parameter data type.
    int lastHistoryIndex;             ///< Last history index.
    std::string History[HISTORY_CAP]; ///< Parameter history.
    SensorRestrictions Restrictions;  ///< Parameter restrictions.
};

/**
 * @class BaseSensor
 * @brief Abstract base class for sensors.
 *
 * Defines common properties and virtual methods for sensor initialization, configuration,
 * updating, and printing. It also provides a helper method, printBasicInfo(), that prints
 * common sensor details.
 */
class BaseSensor
{
protected:
    bool redrawPending = true;  ///< Flag to indicate if sensor needs to be redrawn.
    bool isConfigsSync = false; ///< Flag to indicate if sensor congig is synchronized with real sensor.
    bool isValuesSync = false;  ///< Flag to indicate if sensor values is synchronized with real sensor.

    std::unordered_map<std::string, SensorParam> Values;            ///< Sensor values.
    std::unordered_map<std::string, SensorParam> Configs;          ///< Sensor configurations.
    std::vector<std::string> Pins;                                 ///< Sensor pins.
    std::string AllowedPins;                                       ///< Allowed sensor pins, enter as list of values separated by ",".

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
            Status = SensorStatus::OK;
        }
        else if (status == "-1")
        {
            Status = SensorStatus::ERROR;
        }
        else if (status == "0")
        {
            Status = SensorStatus::OFFLINE;
        }
    }

    /**
     * @brief Set sensor status.
     *
     * This function sets the sensor status based on the given status.
     *
     * @param status The status.
     */
    void setStatus(SensorStatus status)
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
            throw SensorSynchronizationFailException("BaseSensor::syncConfigs", response.error);
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
                throw SensorSynchronizationFailException("BaseSensor::syncValues", response.error);
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
    bool checkRestrictions(std::string value, const SensorParam &param)
    {
        SensorRestrictions restrictions = param.Restrictions;
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
            throw InvalidDataTypeException("BaseSensor::checkRestrictions", e.what());
        }

        return true;
    }

public:
    std::string UID;         ///< Unique sensor identifier.
    SensorStatus Status;     ///< Sensor status.
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
    bool operator==(const BaseSensor &sensor) const
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
     * @brief Constructs a new BaseSensor object.
     */
    BaseSensor() : UID("DummySensor"), Status(SensorStatus::OK)
    {
        LastError = "";

        init();
    }

    /**
     * @brief Constructs a new BaseSensor object.
     *
     * @param uid The unique sensor identifier.
     */
    BaseSensor(std::string uid) : UID(uid), Status(SensorStatus::OK)
    {
        LastError = "";

        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseSensor()
    {
    }

    std::unordered_map<std::string, SensorParam> getValues() const { return Values; }
    std::vector<std::string> getValuesKeys() const
    {
        std::vector<std::string> keys;
        for (const auto &pair : Values)
        {
            keys.push_back(pair.first);
        }
        return keys;
    }
    std::unordered_map<std::string, SensorParam> getConfigs() const { return Configs; }
    std::vector<std::string> getConfigsKeys() const
    {
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
    SensorStatus getStatus() const { return Status; }

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
     * @brief Connect the sensor to its assigned pins.
     * 
     */
    bool connect() 
    {
        std::string pins = getPins();
        if(pins.empty()) {
            throw SensorPinAssignmentException("connectSensor", "No pins assigned to sensor.");
        }

        auto response = Protocol::connect(UID, pins);
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw SensorConnectionFailException("BaseSensor::connect", response.error);
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
            throw SensorConnectionFailException("BaseSensor::disconnect", response.error);
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
            throw ConfigurationNotFoundException("BaseSensor::getConfig", "Configuration not found for key: " + key);
        }

        try
        {
            return convertStringToType<T>(value);
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("BaseSensor::getConfig", e.what());
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
            Configs[key].Value = value;
        }
        else
        {
            throw ConfigurationNotFoundException("BaseSensor::setConfig", "Configuration not found for key: " + key);
        }

        isConfigsSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
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
            throw ValueNotFoundException("BaseSensor::getValue", "Value not found for key: " + key);
        }

        try
        {
            return convertStringToType<T>(value);
        }
        catch (const std::exception &e)
        {
            throw InvalidDataTypeException("BaseSensor::getValue", e.what());
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
            Values[key].Value = value;
        }
        else
        {
            throw ValueNotFoundException("BaseSensor::setValue", "Value not found for key: " + key);
        }
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
            setStatus(SensorStatus::ERROR);
        }
        else
        {
            setStatus(SensorStatus::OK);
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

        throw ValueNotFoundException("BaseSensor::getHistory", "Value not found for key: " + key);
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
        isValuesSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
        if (!isConfigsSync)
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

        if (!isValuesSync)
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

        return isValuesSync && isConfigsSync;
    }

    /**
     * @brief Add configuration parameter to the sensor.
     *
     * @param key The key of the configuration parameter.
     * @param param The configuration parameter to add.
     */
    void addConfigParameter(const std::string &key, const SensorParam &param)
    {
        try
        {
            Configs[key] = param;
        }
        catch (const std::exception &e)
        {
            throw InvalidConfigurationException("BaseSensor::addConfigParameter", e.what());
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
                        throw InvalidValueException("BaseSensor::config", "Value " + value + " for key " + c.first + " does not meet restrictions.");
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
    void addValueParameter(const std::string &key, const SensorParam &param)
    {
        try
        {
            Values[key] = param;
        }
        catch (const std::exception &e)
        {
            throw InvalidValueException("BaseSensor::addValueParameter", new Exception(e));
        }

        isValuesSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
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
                        throw InvalidValueException("BaseSensor::update", "Value " + value + " for key " + c.first + " does not meet restrictions.");
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
        logMessage("Sensor UID: %s\n", UID.c_str());
        logMessage("\tSensor Type: %s\n", Type.c_str());
        logMessage("\tSensor Description: %s\n", Description.c_str());
        logMessage("\tSensor Status: %d\n", Status);
        logMessage("\tSensor Error: %s\n", getError().c_str());
        logMessage("\tSensor Configurations:\n");
        for (auto &c : Configs)
        {
            logMessage("\t\t%s: %s %s\n", c.first.c_str(), c.second.Value.c_str(), c.second.Unit.c_str());
        }
        logMessage("\tSensor Values:\n");
        for (auto &v : Values)
        {
            logMessage("\t\t%s: %s %s\n", v.first.c_str(), v.second.Value.c_str(), v.second.Unit.c_str());
        }
        logMessage("\tSensor Pins: %s\n", getPins().c_str());
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
 * @tparam T The sensor type, which must be derived from BaseSensor.
 * @param uid The unique sensor identifier.
 * @return T* Pointer to the newly created sensor.
 * @throws SensorInitializationFailException if sensor initialization fails.
 */
template <typename T>
T *createSensor(std::string uid)
{
    static_assert(std::is_base_of<BaseSensor, T>::value, "T must be derived from BaseSensor");

    T *sensor = nullptr;
    try
    {
        sensor = new T(uid);
    }
    catch (const std::exception &ex)
    {
        logMessage("Error during sensor initialization: %s\n", ex.what());
        delete sensor;
        throw SensorInitializationFailException("createSensor", "Error during sensor initialization.", new Exception(ex));
    }

    logMessage("Sensor [%s]:%s created successfully.\n", sensor->UID.c_str(), sensor->Type.c_str());
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
bool configSensor(BaseSensor *sensor, const std::string &config);

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
bool updateSensor(BaseSensor *sensor, const std::string &update);

/**
 * @brief Prints detailed information about the sensor.
 *
 * This function prints sensor details by calling the sensor's own printSensor() method,
 * which includes both basic sensor info and any additional sensor-specific data.
 *
 * @param sensor Pointer to the sensor whose information is to be printed.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
void printSensor(BaseSensor *sensor);

/**
 * @brief Synchronizes the sensor with the real sensor.
 *
 * This function synchronizes the sensor with the real sensor by calling the sensor's synchronize() method.
 *
 * @param sensor Pointer to the sensor to be synchronized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool syncSensor(BaseSensor *sensor);

/**
 * @brief Initializes the sensor.
 *
 * This function initializes the sensor by calling the sensor's init() method.
 *
 * @param sensor Pointer to the sensor to be initialized.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool initSensor(BaseSensor *sensor);

/**
 * @brief Connect the sensor to the specified pins.
 * 
 * @param sensor Pointer to the sensor to be connected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool connectSensor(BaseSensor *sensor);

/**
 * @brief Disconnect the sensor from its current pins.
 * 
 * @param sensor Pointer to the sensor to be disconnected.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
bool disconnectSensor(BaseSensor *sensor);

#endif // BASE_SENSOR_HPP
