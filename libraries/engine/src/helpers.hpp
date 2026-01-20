/*
* Copyright 2025 MTA
* Author: Ing. Jiri Konecny
*/

#ifndef __HELPERS_H_
#define __HELPERS_H_

/*********************
 *      INCLUDES
 *********************/
#include "exceptions/data_exceptions.hpp" ///< Data related exceptions.

#include <string>
#include <vector>
#include <type_traits>      ///< For is_same
#include <algorithm>      ///< For std::replace, std::transform
#include <unordered_map>   ///< For std::unordered_map
#include <sstream>        ///< For std::stringstream
/**********************
 *      TYPEDEFS
 **********************/


/*********************
 *      DECLARES
 *********************/

/**
 * @brief Parse parameters from a query string.
 * 
 * This function parses a query string and returns a map of key-value pairs.
 * @param message The input query string.
 * @param caseSensitive Whether the parsing should be case-sensitive.
 * @return A map of key-value pairs.
 */
std::unordered_map<std::string, std::string> parseParamsFromString(const std::string& message, bool caseSensitive);

/**
 * @brief Get value from update string.
 * 
 * This function extracts the value of a given key from an update string.
 * 
 * @param str The input string.
 * @param key The key to search for.
 * @return The value corresponding to the key, if exist.
 */
std::string getValueFromKeyValueLikeString(std::string str, std::string key, char separator);

/**
 * @brief Split string by separator.
 * 
 * This function splits a string into a vector of strings using a specified separator.
 * 
 * @param str The input string.
 * @param separator The separator character.
 * @return A vector of strings.
 */
std::vector<std::string> splitString(std::string str, char separator);

/**
 * @brief Convert string to type.
 * 
 * This function converts a string to a specified type.
 * 
 * @param str The string value to convert.
 * @return The converted value or default value.
 */
template <typename T>
T convertStringToType(const std::string &str);

// Specialization for int
template <>
int convertStringToType<int>(const std::string &str);

// Specialization for double
template <>
double convertStringToType<double>(const std::string &str);

// Specialization for float
template <>
float convertStringToType<float>(const std::string &str);

// Specialization for std::string
template <>
std::string convertStringToType<std::string>(const std::string &str);

/**
 * @brief Check if value is in vector.
 * 
 * This function checks if a value exists in a vector.
 * 
 * @param vec The input vector.
 * @param value The value to search for.
 * @return True if the value is found, false otherwise.
 */
bool isInVector(const std::vector<std::string>& vec, const std::string& value);

#endif //__HELPERS_H_