/**
 * @file sensor_factory.hpp
 * @brief Declares the sensor factory functions, for building list of real-to-digital sensors.
 * 
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#ifndef SENSOR_FACTORY_HPP
#define SENSOR_FACTORY_HPP

#include "sensors.hpp"

/**
 * @brief Create a sensor by type.
 * 
 * This function creates a sensor object based on the given type and unique identifier.
 * 
 * @param type The sensor type.
 * @param uid The unique sensor identifier.
 * @return The sensor object.
 */
BaseDevice* createSensorByType(std::string type, std::string uid);

/**
 * @brief Deprecated fixed catalog of sensors.
 * 
 * This function is kept only for backward compatibility. Runtime boot now loads
 * the catalog from JSON on SD card and this fixed list should not be used for new work.
 * 
 * @param memory The list of sensors.
 */
void createSensorList(std::vector<BaseDevice*> &memory);

/**
 * @brief Deprecated string-based sensor catalog factory.
 * 
 * This function is kept only for backward compatibility. Runtime boot now loads
 * the catalog from JSON on SD card and this path should not be used for new work.
 * 
 * @param memory The list of sensors.
 * @param stringSource The string source.
 */
void createSensorList(std::vector<BaseDevice*> &memory, std::string stringSource);

#endif // SENSOR_FACTORY_HPP
