/**
 * @file pin_structure.hpp
 * @brief Pin structure definition with locking mechanism
 *
 * This header defines the Pin structure and related enums for managing
 * hardware pin states, assignments, and locking mechanisms.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef PIN_STRUCTURE_HPP
#define PIN_STRUCTURE_HPP

#include "../sensors/base_sensor.hpp"
#include <string>

/**
 * @enum PinState
 * @brief Enumeration representing possible pin states
 */
enum class PinState {
    AVAILABLE,    ///< Pin is available (green)
    USED,         ///< Pin has an assigned sensor (red)  
    LOCKED        ///< Pin is locked and cannot be used (gray)
};

/**
 * @enum PinLockReason
 * @brief Enumeration representing reasons why a pin might be locked
 */
enum class PinLockReason {
    NONE,            ///< No lock reason
    SYSTEM_RESERVED, ///< Pin reserved by system (display, touch, etc.)
    USER_LOCKED,     ///< Pin manually locked by user
    HARDWARE_ISSUE   ///< Pin has hardware problems
};

/**
 * @struct VirtualPin
 * @brief Structure representing a hardware pin with state management
 */
struct VirtualPin {
    int pinNumber;                    ///< Physical pin number (GPIO)
    PinState state;                   ///< Current pin state
    BaseSensor* assignedSensor;       ///< Pointer to assigned sensor (nullptr if none)
    bool locked;                      ///< Whether pin is locked
    PinLockReason lockReason;         ///< Reason for locking
    std::string customName;           ///< Optional custom name for pin
    std::string lockDescription;      ///< Description of why pin is locked
    
    /**
     * @brief Default constructor (creates invalid pin)
     */
    VirtualPin() : pinNumber(-1)
         , state(PinState::LOCKED)
         , assignedSensor(nullptr)
         , locked(true)
         , lockReason(PinLockReason::SYSTEM_RESERVED)
         , customName("")
         , lockDescription("Invalid/Uninitialized pin") {}
    
    /**
     * @brief Constructor
     * @param pin Physical pin number
     * @param isLocked Whether pin should be locked initially
     * @param reason Lock reason if locked
     * @param description Optional description
     */
    VirtualPin(int pin, bool isLocked = false, PinLockReason reason = PinLockReason::NONE, 
        const std::string& description = "")
        : pinNumber(pin)
        , state(isLocked ? PinState::LOCKED : PinState::AVAILABLE)
        , assignedSensor(nullptr)
        , locked(isLocked)
        , lockReason(reason)
        , customName("")
        , lockDescription(description) {}
    
    /**
     * @brief Check if pin is available for assignment
     * @return True if pin can be assigned a sensor
     */
    bool isAvailable() const {
        return !locked && state == PinState::AVAILABLE;
    }

    /**
     * @brief Check if pin has an assigned sensor
     * @return True if pin has a sensor assigned
     */
    bool isAssigned() const {
        return assignedSensor != nullptr;
    }

    bool isLocked() const {
        return locked;
    }
    
    /**
     * @brief Assign a sensor to this pin
     * @param sensor Pointer to sensor to assign
     * @return True if assignment successful
     */
    bool assignSensor(BaseSensor* sensor) {
        if (!isAvailable()) return false;
        if(!sensor) return false;

        assignedSensor = sensor;
        assignedSensor->assignPin(std::to_string(pinNumber));
        state = PinState::USED;
        return true;
    }
    
    /**
     * @brief Unassign sensor from this pin
     */
    void unassignSensor() {
        assignedSensor = nullptr;
        state = locked ? PinState::LOCKED : PinState::AVAILABLE;
    }
    
    /**
     * @brief Lock the pin with a reason
     * @param reason Reason for locking
     * @param description Optional description
     */
    void lockPin(PinLockReason reason, const std::string& description = "") {
        locked = true;
        lockReason = reason;
        lockDescription = description;
        if (assignedSensor) {
            unassignSensor();
        }
        state = PinState::LOCKED;
    }
    
    /**
     * @brief Unlock the pin
     */
    void unlockPin() {
        locked = false;
        lockReason = PinLockReason::NONE;
        lockDescription = "";
        state = assignedSensor ? PinState::USED : PinState::AVAILABLE;
    }
    
    /**
     * @brief Get display name for pin
     * @return String representation for display
     */
    std::string getDisplayName() const {
        if (!customName.empty()) {
            return customName;
        }
        if (assignedSensor) {
            return assignedSensor->getName();
        }
        return "Pin " + std::to_string(pinNumber);
    }
    
    /**
     * @brief Get color code for pin state visualization
     * @return LVGL color hex value
     */
    uint32_t getStateColor() const {
        switch (state) {
            case PinState::AVAILABLE: return 0x009BFF; // Blue
            case PinState::USED:      return 0x4CB9FF; // Light blue
            case PinState::LOCKED:    return 0x808080; // Gray
            default:                  return 0x808080; // Default gray
        }
    }
};

#endif // PIN_STRUCTURE_HPP