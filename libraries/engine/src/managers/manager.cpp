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
        initMessenger();
        loadConfigFile(configFile);

        logMessage("\tinitializing of protocol...\n");
        ResponseStatus response;
        for (size_t i = 0; i < SensorManager::MAX_INIT_ATTEMPTS; i++)
        {
            response = Protocol::init(APP_NAME, DB_VERSION);
            if (response.status == ResponseStatusEnum::OK)
            {
                logMessage("\t\tProtocol initialized successfully!\n");
                break;
            }
            logMessage("\t\tProtocol initialization failed, retrying...\n");
            delay_ms(500);
        }
        if (response.status == ResponseStatusEnum::ERROR)
        {
            throw SensorInitializationFailException("SensorManager::init", response.error, ErrorCode::CRITICAL_ERROR_CODE);
        }
        logMessage("\tdone!\n");
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
    //TODO:disconnect existing connections first
    bool result = true;
    for (auto virtualPin : PinMap) {
        if(virtualPin.isAssigned()) {
            //First disconnect if already connected
            disconnectSensor(virtualPin.assignedSensor);
            //Reassign pin
            virtualPin.assignedSensor->assignPin(std::to_string(virtualPin.pinNumber));
        }
    }

    for (auto virtualPin : PinMap) {
        if(virtualPin.isAssigned()) {
            result &= connectSensor(virtualPin.assignedSensor);
        }
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

    PinMap[activePin].unassignSensor();
    return true;
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