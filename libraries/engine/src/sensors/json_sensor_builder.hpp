#ifndef JSON_SENSOR_BUILDER_HPP
#define JSON_SENSOR_BUILDER_HPP

#include "base_sensor.hpp"

#include <string>
#include <vector>

struct SensorCatalog
{
    std::vector<BaseSensor *> sensors;
    std::string version;
    std::string application;
};

SensorCatalog buildSensorCatalogFromSdFile(const std::string &filePath);

#endif // JSON_SENSOR_BUILDER_HPP
