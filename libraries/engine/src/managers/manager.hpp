/**
 * @file manager.hpp
 * @brief Declaration of the manager
 *
 * This header defines the manager class for managing sensors.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */
#ifndef MANAGER_HPP
#define MANAGER_HPP

#define NUM_PINS 18

#include <vector>
#include <cstddef>
#include <string>
#include <array>
#include "expt.hpp"

#include "../sensors/base_sensor.hpp"
#include "pin_structure.hpp"

/**
 * @enum ManagerStatus
 * @brief Enumeration representing possible manager statuses.
 *
 * - STOPPED: Manager has stopped.
 * - RUNNING: Manager is running.
 * - ERROR: Manager has encountered an error.
 */
enum class ManagerStatus
{
    READY = 0,  ///< Manager is ready.
    STOPPED = 1, ///< Manager has stopped.
    RUNNING = 2, ///< Manager is running.
    ERROR = -1  ///< Manager has encountered an error.
};


/**
 * @class SensorManager
 * @brief Class for managing sensors and their pin assignments.
 *
 * Provides methods for adding, accessing, synchronizing, and assigning sensors to pins.
 * Maintains a list of sensors and a mapping of sensors to hardware pins.
 */
class SensorManager {
private:
    std::array<VirtualPin, NUM_PINS> PinMap; ///< Mapping of pins to sensors
    std::vector<BaseSensor*> Sensors;         ///< List of all managed sensors
    std::vector<BaseSensor*> SelectedSensors; ///< List of fixed sensors (from config file)

    size_t currentIndex = 0;                      ///< Index of the current sensor
    BaseSensor* currentWikiSensor = nullptr;    ///< Pointer to the current chosen wiki sensor

    bool initialized = false;                 ///< Initialization state flag
    ManagerStatus Status = ManagerStatus::STOPPED; ///< Current status of the manager

    std::string configFilePath;          ///< Path to configuration file
    std::string DB_VERSION = "";    ///< Database version
    std::string APP_NAME = ""; ///< Application name

public:
    const static uint8_t MAX_INIT_ATTEMPTS = 5; ///< Maximum initialization attempts
    /**
     * @brief Private constructor for singleton pattern
     */
    SensorManager();

    /**
     * @brief Destructor
     */
    ~SensorManager();

    /**
     * @brief Load configuration file
     * @param configFile Path to configuration file
     */
    void loadConfigFile(std::string configFile);

    /**
     * @brief Initialize the manager with a configuration file
     * @param configFile Path to the configuration file
     * @return True if initialization was successful, false otherwise
     */
    bool init(std::string configFile = "");

    /**
     * @brief Check if the manager has been initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }

    /**
     * @brief Check if the manager is currently running
     * @return True if running, false otherwise
     */
    bool isRunning(){ return Status == ManagerStatus::RUNNING; }

    /**
     * @brief Set the running status of the manager
     * @param running True to set status to RUNNING, false to set to STOPPED
     */
    void setRunning(bool running) { Status = running ? ManagerStatus::RUNNING : ManagerStatus::STOPPED; }

    /**
     * @brief Check if any sensor requires a redraw
     * @return True if any sensor has redrawPending flag set, false otherwise
     */
    bool isRedrawPending() const {
        for (const auto* sensor : Sensors) {
            if (sensor && sensor->getRedrawPending()) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Get a sensor by its unique ID
     * @param uid Unique identifier string
     * @return Pointer to the sensor, or nullptr if not found
     */
    BaseSensor* getSensor(std::string uid);

    /**
     * @brief Add a sensor to the manager
     * @param sensor Pointer to the sensor to add
     */
    void addSensor(BaseSensor* sensor);

    /**
     * @brief Reset the pin map, unassigning all sensors from pins
     */
    void resetPinMap();

    /**
     * @brief Synchronize a sensor by ID
     * @param id Unique identifier string
     */
    bool sync(std::string id);

    /**
     * @brief Print information about a sensor by UID
     * @param uid Unique identifier string
     */
    void print(std::string uid);

    /**
     * @brief Print information about all sensors
     */
    void print();

    /**
     * @brief Resynchronize all sensors
     */
    bool resync();

    /**
     * @brief Connect sensors to pins (bulk operation)
     */
    bool connect();

    /**
     * @brief Erase all sensors and pin assignments
     */
    void erase();

    // --- Pin mapping ---

    /**
     * @brief Assign a sensor to the currently active pin
     * @param sensor Pointer to the sensor to assign
     */
    bool assignSensorToPin(BaseSensor* sensor, int activePin);

    /**
     * @brief Unassign the sensor from the currently active pin
     */
    bool unassignSensorFromPin(int activePin);

    /**
     * @brief Get the sensor assigned to a specific pin
     * @param pinIndex Index of the pin (0-based)
     * @return Pointer to the assigned sensor, or nullptr if none
     */
    BaseSensor* getAssignedSensor(size_t pinIndex) const;

    /**
     * @brief Get the GPIO pin number for a specific pin index
     * @param pinIndex Index of the pin (0-based)
     * @return GPIO pin number, or -1 if invalid index
     */
    int getPinNumber(size_t pinIndex) const;

    /**
     * @brief Check if a pin index is available for assignment
     * @param pinIndex Index of the pin (0-based)
     * @return True if pin is available, false if occupied or invalid
     */
    bool isPinAvailable(size_t pinIndex) const;

    /**
     * @brief Check if a pin index is locked
     * @param pinIndex Index of the pin (0-based)
     * @return True if pin is locked, false if available or invalid
     */
    bool isPinLocked(size_t pinIndex) const;

    /**
     * @brief Get read-only access to the list of sensors
     * @return Const reference to the vector of sensor pointers
     */
    const std::vector<BaseSensor*>& getSensors() const { return Sensors; }

    /**
     * @brief Get read-only access to the pin map
     * @return Const reference to the array of pin assignments
     */
    const std::array<VirtualPin, NUM_PINS>& getPinMap() const { return PinMap; }

    // --- Sensors mapping ---
    
    /**
     * @brief Select sensors that are assigned to pins into the SelectedSensors list
     */
    void selectSensorsFromPinMap(); 

    /**
     * @brief Get the currently selected sensor
     * @return Pointer to the current sensor
     */
    BaseSensor* getCurrentSensor();

    /**
     * @brief Get the currently chosen sensor used in wiki for menu sensor assignment
     * @return Pointer to the current chosen sensor
     */
    BaseSensor* getCurrentWikiSensor();

    /**
     * @brief Set the currently chosen sensor used in wiki for menu sensor assignment
     * @param sensor Pointer to the sensor to set as current chosen
     */
    void setCurrentWikiSensor(BaseSensor* sensor);

    /**
     * @brief Get access to the current sensor index
     * @return Reference to the current index
     */
    size_t& getCurrentIndex() { return currentIndex; }

    /**
     * @brief Reset the current sensor index to zero
     */
    void resetCurrentIndex() { currentIndex = 0; }

    /**
     * @brief Move to the next sensor in the list (wraps around)
     * @return Reference to the updated current index
     */
    BaseSensor* nextSensor();

    /**
     * @brief Move to the previous sensor in the list (wraps around)
     * @return Reference to the updated current index
     */
    BaseSensor* previousSensor();
};

#endif // MANAGER_HPP 