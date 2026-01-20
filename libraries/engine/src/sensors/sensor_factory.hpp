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
BaseSensor* createSensorByType(std::string type, std::string uid);

/**
 * @brief Create a list of sensors.
 * 
 * This function creates a fixed list of sensors.
 * 
 * @param memory The list of sensors.
 */
void createSensorList(std::vector<BaseSensor*> &memory);

/**
 * @brief Create a list of sensors.
 * 
 * This function creates a list of sensors based on the given string source.
 * 
 * @param memory The list of sensors.
 * @param stringSource The string source.
 */
void createSensorList(std::vector<BaseSensor*> &memory, std::string stringSource);

#endif // SENSOR_FACTORY_HPP