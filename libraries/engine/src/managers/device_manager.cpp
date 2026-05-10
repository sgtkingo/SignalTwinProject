/**
 * @file device_manager.cpp
 * @brief Definition of the manager 
 * 
 * This source defines the manager functions and implementations.
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny, Ondřej Wrubel
 */

/*********************
 *      INCLUDES
 *********************/

#include <sstream>
#include <utility>
#include "device_manager.hpp"
#include "helpers.hpp"

DeviceManager::DeviceManager(DeviceCatalog &catalog) : catalog(catalog) {
}

DeviceManager::~DeviceManager() {
}

std::vector<BaseDevice *> DeviceManager::collectAssignedDevicesFromPinMap() const
{
    std::vector<BaseDevice *> uniqueDevices;

    for (const auto &virtualPin : PinMap) {
        if (!virtualPin.isAssigned()) {
            continue;
        }

        if (!std::count(uniqueDevices.begin(), uniqueDevices.end(), virtualPin.assignedDevice)) {
            uniqueDevices.push_back(virtualPin.assignedDevice);
        }
    }

    return uniqueDevices;
}

void DeviceManager::resetPinState(size_t pinIndex)
{
    if (!isValidPinIndex(pinIndex)) {
        return;
    }

    detachDeviceFromPin(pinIndex);
    PinMap[pinIndex].pinNumber = static_cast<int>(pinIndex);
    PinMap[pinIndex].locked = false;
    PinMap[pinIndex].lockReason = PinLockReason::NONE;
    PinMap[pinIndex].lockDescription = "";
    PinMap[pinIndex].state = PinState::AVAILABLE;
}

void DeviceManager::applyAssignedPinsToDevices() const
{
    for (const auto &virtualPin : PinMap) {
        if (virtualPin.isAssigned()) {
            virtualPin.assignedDevice->assignPin(std::to_string(virtualPin.pinNumber));
        }
    }
}

void DeviceManager::disconnectAssignedDevices(const std::vector<BaseDevice *> &devices) const
{
    for (BaseDevice *device : devices) {
        disconnectDevice(device);
    }
}

bool DeviceManager::connectAssignedDevices(const std::vector<BaseDevice *> &devices) const
{
    bool result = true;
    for (BaseDevice *device : devices) {
        result &= connectDevice(device);
    }
    return result;
}

bool DeviceManager::isValidPinIndex(size_t pinIndex) const
{
    return pinIndex < NUM_PINS;
}

VirtualPin *DeviceManager::getPinState(size_t pinIndex)
{
    if (!isValidPinIndex(pinIndex)) {
        return nullptr;
    }

    return &PinMap[pinIndex];
}

const VirtualPin *DeviceManager::getPinState(size_t pinIndex) const
{
    if (!isValidPinIndex(pinIndex)) {
        return nullptr;
    }

    return &PinMap[pinIndex];
}

bool DeviceManager::detachDeviceFromPin(size_t pinIndex)
{
    VirtualPin *pin = getPinState(pinIndex);
    if (!pin) {
        return false;
    }

    BaseDevice *device = pin->assignedDevice;
    if (device) {
        device->unassignPin(std::to_string(pin->pinNumber));
    }

    pin->unassignDevice();
    return true;
}

bool DeviceManager::init() {
    if(initialized)
    {
        erase();
    }

    initialized = false;
    Status = ManagerStatus::ERROR;
    if (!catalog.isInitialized()) {
        throw DeviceInitializationFailException("DeviceManager::init", "Device catalog must be initialized before runtime manager init.");
    }

    Status = ManagerStatus::READY;
    resetPinMap();
    logMessage("Initialization done!\n");
    return initialized = true;
}

bool DeviceManager::ensureProtocolInitialized()
{
    if (Protocol::isInitialized()) {
        return true;
    }

    return initializeProtocolConnection();
}

bool DeviceManager::initializeProtocolConnection()
{
    logMessage("\tinitializing protocol on demand...\n");

    ResponseStatus response {ResponseStatusEnum::ERROR, "Protocol initialization failed", {}};
    for (size_t i = 0; i < DeviceManager::MAX_INIT_ATTEMPTS; i++)
    {
        response = Protocol::init(catalog.getApplication(), catalog.getVersion());
        if (response.status == ResponseStatusEnum::OK)
        {
            logMessage("\t\tProtocol initialized successfully!\n");
            return true;
        }

        logMessage("\t\tProtocol initialization failed, retrying...\n");
        delay_ms(500);
    }

    logMessage("\t\tProtocol initialization failed permanently: %s\n", response.error.c_str());
    return false;
}

BaseDevice* DeviceManager::getDevice(std::string uid) {
    return catalog.getDevice(uid);
}

void DeviceManager::addDevice(BaseDevice* device) {
    (void)device;
}

bool DeviceManager::sync(std::string id) {
    BaseDevice* device = getDevice(id);
    if (device) return syncDevice(device);

    return false;
}

void DeviceManager::print(std::string uid) {
    BaseDevice* device = getDevice(uid);
    printDevice(device);
}

void DeviceManager::print(BaseDevice *device) {
    printDevice(device);
}

bool DeviceManager::resync(BaseDevice *device) 
{
    if(!isRunning()) return false;
    if(device) {
        device->requestRuntimeUpdate();
    }
    return syncDevice(device);
}

bool DeviceManager::connect() 
{
    if (!ensureProtocolInitialized()) {
        return false;
    }

    const std::vector<BaseDevice *> assignedDevices = collectAssignedDevicesFromPinMap();
    disconnectAssignedDevices(assignedDevices);
    applyAssignedPinsToDevices();
    return connectAssignedDevices(assignedDevices);
}

void DeviceManager::erase() {
    resetPinMap();
}

/////////////////////////
// Pin management
/////////////////////////

void DeviceManager::resetPinMap() {
    for (size_t i = 0; i < NUM_PINS; ++i) {
        resetPinState(i);
    }
}

bool DeviceManager::assignDeviceToPin(BaseDevice* device, int activePin) {
    VirtualPin *pin = getPinState(static_cast<size_t>(activePin));
    if (!pin) return false;

    return pin->assignDevice(device);
}

bool DeviceManager::unassignDeviceFromPin(int activePin) {
    return detachDeviceFromPin(static_cast<size_t>(activePin));
}

bool DeviceManager::unassignAllPinsForDevice(BaseDevice *device)
{
    if (!device) {
        return false;
    }

    bool changed = false;
    for (size_t i = 0; i < NUM_PINS; ++i) {
        if (PinMap[i].assignedDevice == device) {
            unassignDeviceFromPin(static_cast<int>(i));
            changed = true;
        }
    }

    return changed;
}

BaseDevice* DeviceManager::getAssignedDevice(size_t pinIndex) const {
    const VirtualPin *pin = getPinState(pinIndex);
    return pin ? pin->assignedDevice : nullptr;
}

int DeviceManager::getPinNumber(size_t pinIndex) const {
    const VirtualPin *pin = getPinState(pinIndex);
    return pin ? pin->pinNumber : -1;
}

bool DeviceManager::isPinAvailable(size_t pinIndex) const {
    const VirtualPin *pin = getPinState(pinIndex);
    return pin ? pin->isAvailable() : false;
}

bool DeviceManager::isPinLocked(size_t pinIndex) const {
    const VirtualPin *pin = getPinState(pinIndex);
    return pin ? pin->isLocked() : false;
}

bool DeviceManager::hasAssignedDevices() const
{
    for (const auto &pin : PinMap) {
        if (pin.isAssigned()) {
            return true;
        }
    }
    return false;
}
