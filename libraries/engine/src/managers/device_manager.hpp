/**
 * @file device_manager.hpp
 * @brief Declaration of the manager
 *
 * This header defines the manager class for managing devices.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */
#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#define NUM_PINS 18

#include <vector>
#include <cstddef>
#include <string>
#include <array>
#include "expt.hpp"

#include "../devices/base_device.hpp"
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
 * @class DeviceManager
 * @brief Class for managing devices and their pin assignments.
 *
 * Provides methods for adding, accessing, synchronizing, and assigning devices to pins.
 * Maintains a catalog of devices and a mapping of devices to hardware pins.
 */
class DeviceManager {
private:
    std::array<VirtualPin, NUM_PINS> PinMap;     ///< Mapping of pins to devices.
    std::vector<BaseDevice*> Devices;            ///< Complete runtime device catalog.
    std::vector<BaseDevice*> SelectedDevices;    ///< Devices selected for the current visualization session.

    size_t currentIndex = 0;                         ///< Index of the current visualized device.
    BaseDevice* currentSelectionDevice = nullptr;    ///< Device currently highlighted in Selection.
    BaseDevice* currentLibraryDevice = nullptr;      ///< Device currently highlighted in Library.

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
    DeviceManager();

    /**
     * @brief Destructor
     */
    ~DeviceManager();

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
     * @brief Check if any device requires a redraw.
     * @return True if any device has `redrawPending` flag set, false otherwise.
     */
    bool isRedrawPending() const {
        for (const auto* device : Devices) {
            if (device && device->getRedrawPending()) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Get a device by its unique ID.
     * @param uid Unique identifier string
     * @return Pointer to the device, or `nullptr` if not found.
     */
    BaseDevice* getDevice(std::string uid);

    /**
     * @brief Add a device to the manager.
     * @param device Pointer to the device to add.
     */
    void addDevice(BaseDevice* device);

    /**
     * @brief Reset the pin map, unassigning all devices from pins.
     */
    void resetPinMap();

    /**
     * @brief Synchronize a device by ID.
     * @param id Unique identifier string
     */
    bool sync(std::string id);

    /**
     * @brief Print information about a device by UID.
     * @param uid Unique identifier string
     */
    void print(std::string uid);

    /**
     * @brief Print information about the current device.
     */
    void print();

    /**
     * @brief Resynchronize the currently visualized device.
     */
    bool resync();

    /**
     * @brief Initialize protocol lazily when the user reaches the connection step.
     * @return True if protocol is initialized or already ready
     */
    bool ensureProtocolInitialized();

    /**
     * @brief Connect assigned devices to pins (bulk operation).
     */
    bool connect();

    /**
     * @brief Erase all devices and pin assignments.
     */
    void erase();

    // --- Pin mapping ---

    /**
     * @brief Assign a device to the currently active pin.
     * @param device Pointer to the device to assign.
     */
    bool assignDeviceToPin(BaseDevice* device, int activePin);

    /**
     * @brief Unassign the device from the currently active pin.
     */
    bool unassignDeviceFromPin(int activePin);

    /**
     * @brief Unassign all pins used by a device in the current session.
     * @param device Pointer to the device to unassign.
     * @return True if at least one pin was unassigned
     */
    bool unassignAllPinsForDevice(BaseDevice *device);

    /**
     * @brief Get the device assigned to a specific pin.
     * @param pinIndex Index of the pin (0-based)
     * @return Pointer to the assigned device, or `nullptr` if none.
     */
    BaseDevice* getAssignedDevice(size_t pinIndex) const;

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
     * @brief Check if any device is assigned to any pin.
     * @return True if there is at least one assigned device.
     */
    bool hasAssignedDevices() const;

    /**
     * @brief Get read-only access to the device catalog.
     * @return Const reference to the vector of device pointers.
     */
    const std::vector<BaseDevice*>& getDevices() const { return Devices; }

    /**
     * @brief Get read-only access to the pin map
     * @return Const reference to the array of pin assignments
     */
    const std::array<VirtualPin, NUM_PINS>& getPinMap() const { return PinMap; }

    // --- Visualization session mapping ---
    
    /**
     * @brief Select assigned devices into the current visualization session list.
     */
    void selectDevicesFromPinMap(); 

    /**
     * @brief Get the currently visualized device.
     * @return Pointer to the current device.
     */
    BaseDevice* getCurrentDevice();

    /**
     * @brief Get the currently highlighted Selection device.
     * @return Pointer to the current selection device.
     */
    BaseDevice* getCurrentSelectionDevice();

    /**
     * @brief Set the currently highlighted Selection device.
     * @param device Pointer to the device to set as current selection.
     */
    void setCurrentSelectionDevice(BaseDevice* device);

    /**
     * @brief Get the current Library device.
     * @return Pointer to the current library device.
     */
    BaseDevice* getCurrentLibraryDevice();

    /**
     * @brief Set the current Library device.
     * @param device Pointer to the device or `nullptr` for new-entity flow.
     */
    void setCurrentLibraryDevice(BaseDevice *device);

    /**
     * @brief Get access to the current device index.
     * @return Reference to the current index.
     */
    size_t& getCurrentIndex() { return currentIndex; }

    /**
     * @brief Reset the current device index to zero.
     */
    void resetCurrentIndex() { currentIndex = 0; }

    /**
     * @brief Move to the next device in the session list (wraps around).
     * @return Pointer to the new current device.
     */
    BaseDevice* nextDevice();

    /**
     * @brief Move to the previous device in the session list (wraps around).
     * @return Pointer to the new current device.
     */
    BaseDevice* previousDevice();
};

#endif // DEVICE_MANAGER_HPP 
