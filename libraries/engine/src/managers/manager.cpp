/**
 * @file manager.cpp
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
#include "manager.hpp"
#include "../sensors/sensor_factory.hpp"
#include "helpers.hpp"

SensorManager::SensorManager() : Sensors(), currentIndex(0) {
}

SensorManager::~SensorManager() {
    for (auto* s : Sensors) delete s;
}

void SensorManager::loadConfigFile(std::string configFile) {
    configFilePath = configFile;
    if (configFile.empty())
    {
        logMessage("Initializing manager via fixed sensors list...\n");
        createSensorList(Sensors);
        return;
    }

    throw Exception("SensorManager::init", "Initialization from config file not implemented yet", ErrorCode::NOT_DEFINED_ERROR);
}

bool SensorManager::init(std::string configFile) {
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

bool SensorManager::ensureProtocolInitialized()
{
    if (Protocol::isInitialized()) {
        return true;
    }

    logMessage("\tinitializing protocol on demand...\n");

    ResponseStatus response {ResponseStatusEnum::ERROR, "Protocol initialization failed", {}};
    for (size_t i = 0; i < SensorManager::MAX_INIT_ATTEMPTS; i++)
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



BaseSensor* SensorManager::getSensor(std::string uid) {
    for (auto* sensor : Sensors) {
        if (sensor->UID == uid) return sensor;
    }
    return nullptr;
}

void SensorManager::addSensor(BaseSensor* sensor) {
    if (sensor) Sensors.push_back(sensor);
}

bool SensorManager::sync(std::string id) {
    BaseSensor* sensor = getSensor(id);
    if (sensor) return syncSensor(sensor);

    return false;
}

void SensorManager::print(std::string uid) {
    BaseSensor* sensor = getSensor(uid);
    printSensor(sensor);
}

void SensorManager::print() {
    BaseSensor* currentSensor = getCurrentSensor();
    printSensor(currentSensor);
}

bool SensorManager::resync() 
{
    if(!isRunning()) return false;

    BaseSensor* currentSensor = getCurrentSensor();
    return syncSensor(currentSensor);
}

bool SensorManager::connect() 
{
    if (!ensureProtocolInitialized()) {
        return false;
    }

    bool result = true;
    std::vector<BaseSensor *> uniqueSensors;

    for (const auto &virtualPin : PinMap) {
        if (!virtualPin.isAssigned()) {
            continue;
        }

        if (!std::count(uniqueSensors.begin(), uniqueSensors.end(), virtualPin.assignedSensor)) {
            uniqueSensors.push_back(virtualPin.assignedSensor);
        }
    }

    for (BaseSensor *sensor : uniqueSensors) {
        disconnectSensor(sensor);
    }

    for (const auto &virtualPin : PinMap) {
        if (virtualPin.isAssigned()) {
            virtualPin.assignedSensor->assignPin(std::to_string(virtualPin.pinNumber));
        }
    }

    for (BaseSensor *sensor : uniqueSensors) {
        result &= connectSensor(sensor);
    }

    return result;
}

void SensorManager::erase() {
    resetPinMap();
    currentIndex = 0;
    for (auto* sensor : Sensors) delete sensor;
    Sensors.clear();
}

/////////////////////////
// Sensor selection management
/////////////////////////

void SensorManager::selectSensorsFromPinMap() {
    SelectedSensors.clear();
    for (const auto& pin : PinMap) {
        if (pin.assignedSensor) {
            if (!std::count(SelectedSensors.begin(), SelectedSensors.end(), pin.assignedSensor))
            {
                SelectedSensors.push_back(pin.assignedSensor);
            }     
        }
    }
    resetCurrentIndex();
}

BaseSensor* SensorManager::getCurrentSensor()
{
    if (SelectedSensors.empty()) return nullptr;

    if (currentIndex < SelectedSensors.size())
    {
        return SelectedSensors[currentIndex];
    }
    return nullptr;
}

BaseSensor* SensorManager::getCurrentWikiSensor(){
    if(!currentWikiSensor){
        return nullptr;
    }
    return currentWikiSensor;
}

void SensorManager::setCurrentWikiSensor(BaseSensor* sensor){
    if(!sensor){
        currentWikiSensor = nullptr;
        return;
    }
    currentWikiSensor = sensor;
}

BaseSensor* SensorManager::nextSensor() { 
    currentIndex = (currentIndex + 1) % SelectedSensors.size();
    return getCurrentSensor();
}

BaseSensor* SensorManager::previousSensor() {
    currentIndex = (currentIndex == 0) ? SelectedSensors.size() - 1 : currentIndex - 1;
    return getCurrentSensor();
}

/////////////////////////
// Pin management
/////////////////////////

void SensorManager::resetPinMap() {
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

bool SensorManager::assignSensorToPin(BaseSensor* sensor, int activePin) {
    if (activePin >= NUM_PINS) return false;

    return PinMap[activePin].assignSensor(sensor);
}

bool SensorManager::unassignSensorFromPin(int activePin) {
    if (activePin >= NUM_PINS) return false;

    BaseSensor *sensor = PinMap[activePin].assignedSensor;
    if (sensor) {
        sensor->unassignPin(std::to_string(PinMap[activePin].pinNumber));
    }
    PinMap[activePin].unassignSensor();
    return true;
}

bool SensorManager::unassignAllPinsForSensor(BaseSensor *sensor)
{
    if (!sensor) {
        return false;
    }

    bool changed = false;
    for (size_t i = 0; i < NUM_PINS; ++i) {
        if (PinMap[i].assignedSensor == sensor) {
            unassignSensorFromPin(static_cast<int>(i));
            changed = true;
        }
    }

    return changed;
}

BaseSensor* SensorManager::getAssignedSensor(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return nullptr;
    return PinMap[pinIndex].assignedSensor;
}

int SensorManager::getPinNumber(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return -1;
    return PinMap[pinIndex].pinNumber;
}

bool SensorManager::isPinAvailable(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return false;
    return PinMap[pinIndex].isAvailable();
}

bool SensorManager::isPinLocked(size_t pinIndex) const {
    if (pinIndex >= NUM_PINS) return false;
    return PinMap[pinIndex].isLocked();
}

bool SensorManager::hasAssignedSensors() const
{
    for (const auto &pin : PinMap) {
        if (pin.isAssigned()) {
            return true;
        }
    }
    return false;
}

BaseSensor* SensorManager::getCurrentLibrarySensor()
{
    return currentLibrarySensor;
}

void SensorManager::setCurrentLibrarySensor(BaseSensor *sensor)
{
    currentLibrarySensor = sensor;
}
