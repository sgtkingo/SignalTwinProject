/**
 * @file sensor_factory.cpp
 * @brief Defines the sensor factory functions, for building list of real-to-digital sensors.
 * 
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#include "sensor_factory.hpp"

void createSensorList(std::vector<BaseSensor*> &memory)
{
    memory.clear();
    //Add sensors here
    memory.push_back(new MicrophoneSensor("mic_001"));
    memory.push_back(new CameraSensor("cam_001"));
    memory.push_back(new CpuTempSensor("cpu_temp"));
    memory.push_back(new DigitalTemperature("S00"));
    memory.push_back(new DHT11("S01"));
    memory.push_back(new PhotoResistor("S15"));
}

void createSensorList(std::vector<BaseSensor*> &memory, std::string stringSource)
{
    memory.clear();
    //Expected format: ?0:ADC&1:ADC&2:TH
    std::vector<std::string> sensorList = splitString(stringSource, '&');
    logMessage("\t(i)Found %d sensors...\n", sensorList.size());
    std::string id;
    std::string type;
    BaseSensor* sensor;

    for (std::string sensorStr: sensorList)
    {
        logMessage("\tProcessing sensor request: %s\n", sensorStr.c_str());
        if (sensorStr.empty())
        {
            continue;
        }
        id = sensorStr.substr(0, sensorStr.find(':'));
        type = sensorStr.substr(sensorStr.find(':') + 1);
        sensor = createSensorByType(type, id);
        if (sensor != nullptr)
        {
            memory.push_back(sensor);
            logMessage("\t(*)Detected known sensor type:%s, sensor with ID:%s added!\n", sensor->Type.c_str(), sensor->UID.c_str());
        }
    }
}

BaseSensor* createSensorByType(std::string type, std::string uid)
{
    return nullptr;
}