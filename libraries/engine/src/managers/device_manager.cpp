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
#include "../devices/json_device_builder.hpp"
#include "helpers.hpp"

namespace
{
constexpr const char *DEFAULT_DEVICE_DB_PATH = "/data/device_db.json";
}

DeviceManager::DeviceManager() : Devices(), currentIndex(0) {
}

DeviceManager::~DeviceManager() {
    for (auto* device : Devices) delete device;
}

void DeviceManager::loadConfigFile(std::string configFile) {
    configFilePath = configFile.empty() ? DEFAULT_DEVICE_DB_PATH : configFile;

    logMessage("Initializing manager via JSON device DB: %s\n", configFilePath.c_str());

    DeviceCatalog catalog = buildDeviceCatalogFromSdFile(configFilePath);
    Devices = std::move(catalog.devices);
    DB_VERSION = catalog.version;
    APP_NAME = catalog.application;

    if (Devices.empty()) {
        throw DeviceInitializationFailException("DeviceManager::loadConfigFile", "Device DB did not produce any devices.");
    }
}

bool DeviceManager::init(std::string configFile) {
    if(initialized)
    {
        erase();
    }

    initialized = false;
    Status = ManagerStatus::ERROR;
    try
    {
        loadConfigFile(configFile);
    }
    catch(...)
    {
        throw;
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

    logMessage("\tinitializing protocol on demand...\n");

    ResponseStatus response {ResponseStatusEnum::ERROR, "Protocol initialization failed", {}};
    for (size_t i = 0; i < DeviceManager::MAX_INIT_ATTEMPTS; i++)
    {
        response = Protocol::init(APP_NAME, DB_VERSION);
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
    for (auto* device : Devices) {
        if (device->UID == uid) return device;
    }
    return nullptr;
}

void DeviceManager::addDevice(BaseDevice* device) {
    if (device) Devices.push_back(device);
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

void DeviceManager::print() {
    BaseDevice* currentDevice = getCurrentDevice();
    printDevice(currentDevice);
}

bool DeviceManager::resync() 
{
    if(!isRunning()) return false;

    BaseDevice* currentDevice = getCurrentDevice();
    return syncDevice(currentDevice);
}

bool DeviceManager::connect() 
{
    if (!ensureProtocolInitialized()) {
        return false;
    }

    bool result = true;
    std::vector<BaseDevice *> uniqueDevices;

    for (const auto &virtualPin : PinMap) {
        if (!virtualPin.isAssigned()) {
            continue;
        }

        if (!std::count(uniqueDevices.begin(), uniqueDevices.end(), virtualPin.assignedSensor)) {
            uniqueDevices.push_back(virtualPin.assignedSensor);
        }
    }

    for (BaseDevice *device : uniqueDevices) {
        disconnectDevice(device);
    }

    for (const auto &virtualPin : PinMap) {
        if (virtualPin.isAssigned()) {
            virtualPin.assignedSensor->assignPin(std::to_string(virtualPin.pinNumber));
        }
    }

    for (BaseDevice *device : uniqueDevices) {
        result &= connectDevice(device);
    }

    return result;
}

void DeviceManager::erase() {
    resetPinMap();
    currentIndex = 0;
    for (auto* device : Devices) delete device;
    Devices.clear();
}

/////////////////////////
// Sensor selection management
/////////////////////////

void DeviceManager::selectDevicesFromPinMap() {
    SelectedDevices.clear();
    for (const auto& pin : PinMap) {
        if (pin.assignedSensor) {
            if (!std::count(SelectedDevices.begin(), SelectedDevices.end(), pin.assignedSensor))
            {
                SelectedDevices.push_back(pin.assignedSensor);
            }     
        }
    }
    resetCurrentIndex();
}

BaseDevice* DeviceManager::getCurrentDevice()
{
    if (SelectedDevices.empty()) return nullptr;

    if (currentIndex < SelectedDevices.size())
    {
        return SelectedDevices[currentIndex];
    }
    return nullptr;
}

BaseDevice* DeviceManager::getCurrentSelectionDevice(){
    if(!currentSelectionDevice){
        return nullptr;
    }
    return currentSelectionDevice;
}

void DeviceManager::setCurrentSelectionDevice(BaseDevice* device){
    if(!device){
        currentSelectionDevice = nullptr;
        return;
    }
    currentSelectionDevice = device;
}

BaseDevice* DeviceManager::nextDevice() { 
    currentIndex = (currentIndex + 1) % SelectedDevices.size();
    return getCurrentDevice();
}

BaseDevice* DeviceManager::previousDevice() {
    currentIndex = (currentIndex == 0) ? SelectedDevices.size() - 1 : currentIndex - 1;
    return getCurrentDevice();
}

/////////////////////////
// Pin management
/////////////////////////

void DeviceManager::resetPinMap() {
    resetCurrentIndex();
    for (size_t i = 0; i < NUM_PINS; ++i) {
        if (PinMap[i].assignedSensor) {
            PinMap[i].assignedSensor->unassignPin(std::to_string(PinMap[i].pinNumber));
        }
        PinMap[i].pinNumber = i;
        PinMap[i].locked = false;

        PinMap[i].unassignSensor();
    }
}

bool DeviceManager::assignDeviceToPin(BaseDevice* device, int activePin) {
    if (activePin >= NUM_PINS) return false;

    return PinMap[activePin].assignSensor(device);
}

bool DeviceManager::unassignDeviceFromPin(int activePin) {
    if (activePin >= NUM_PINS) return false;

    BaseDevice *device = PinMap[activePin].assignedSensor;
    if (device) {
        device->unassignPin(std::to_string(PinMap[activePin].pinNumber));
    }
    PinMap[activePin].unassignSensor();
    return true;
}

bool DeviceManager::unassignAllPinsForDevice(BaseDevice *device)
{
    if (!device) {
        return false;
    }

    bool changed = false;
    for (size_t i = 0; i < NUM_PINS; ++i) {
        if (PinMap[i].assignedSensor == device) {
            unassignDeviceFromPin(static_cast<int>(i));
            changed = true;
        }
    }

    return changed;
}

BaseDevice* DeviceManager::getAssignedDevice(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return nullptr;
    return PinMap[pinIndex].assignedSensor;
}

int DeviceManager::getPinNumber(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return -1;
    return PinMap[pinIndex].pinNumber;
}

bool DeviceManager::isPinAvailable(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return false;
    return PinMap[pinIndex].isAvailable();
}

bool DeviceManager::isPinLocked(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return false;
    return PinMap[pinIndex].isLocked();
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

BaseDevice* DeviceManager::getCurrentLibraryDevice()
{
    return currentLibraryDevice;
}

void DeviceManager::setCurrentLibraryDevice(BaseDevice *device)
{
    currentLibraryDevice = device;
}
