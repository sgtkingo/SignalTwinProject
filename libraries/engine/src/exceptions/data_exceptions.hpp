
/**
 * @file data_exceptions.hpp
 * @brief Specialized exceptions for data and configuration errors.
 *
 * This header defines exception classes for data-related errors, such as configuration not found,
 * invalid configuration, value not found, invalid value, and invalid data type. These exceptions
 * inherit from the base Exception class and provide constructors for different error reporting scenarios.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef DATA_EXCEPTIONS_HPP
#define DATA_EXCEPTIONS_HPP

#include "expt.hpp" ///< For Exception class

/**
 * @class ConfigurationNotFoundException
 * @brief Exception for missing configuration errors.
 *
 * Use this exception to indicate that a required configuration was not found.
 */
class ConfigurationNotFoundException : public Exception
{
public:
    ConfigurationNotFoundException(const std::string &source, Exception* innerException) 
        : Exception("ConfigurationNotFoundException", source, "Configuration not found", ErrorCode::NOT_FOUND, innerException) {}
    ConfigurationNotFoundException(const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) 
        : Exception("ConfigurationNotFoundException", "Unknown source", message, code) {}
    ConfigurationNotFoundException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) 
        : Exception("ConfigurationNotFoundException", source, message, code) {}
    ~ConfigurationNotFoundException(){}
};


/**
 * @class InvalidConfigurationException
 * @brief Exception for invalid configuration errors.
 *
 * Use this exception to indicate that a configuration is invalid.
 */
class InvalidConfigurationException : public Exception
{
public:
    InvalidConfigurationException(const std::string &source, Exception* innerException) 
        : Exception("InvalidConfigurationException", source, "Invalid configuration", ErrorCode::INVALID_VALUE, innerException) {}
    InvalidConfigurationException(const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) 
        : Exception("InvalidConfigurationException", "Unknown source", message, code) {}
    InvalidConfigurationException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) 
        : Exception("InvalidConfigurationException", source, message, code) {}
    ~InvalidConfigurationException(){}
};


/**
 * @class ValueNotFoundException
 * @brief Exception for missing value errors.
 *
 * Use this exception to indicate that a required value was not found.
 */
class ValueNotFoundException : public Exception
{
public:
    ValueNotFoundException(const std::string &source, Exception* innerException) 
        : Exception("ValueNotFoundException", source, "Value not found", ErrorCode::NOT_FOUND, innerException) {}
    ValueNotFoundException(const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) 
        : Exception("ValueNotFoundException", "Unknown source", message, code) {}
    ValueNotFoundException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) 
        : Exception("ValueNotFoundException", source, message, code) {}
    ~ValueNotFoundException(){}
};



/**
 * @class InvalidValueException
 * @brief Exception for invalid value errors.
 *
 * Use this exception to indicate that a value is invalid.
 */
class InvalidValueException : public Exception
{
public:
    InvalidValueException(const std::string &source, Exception* innerException) 
        : Exception("InvalidValueException", source, "Invalid value", ErrorCode::INVALID_VALUE, innerException) {}
    InvalidValueException(const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) 
        : Exception("InvalidValueException", "Unknown source", message, code) {}
    InvalidValueException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) 
        : Exception("InvalidValueException", source, message, code) {}
    ~InvalidValueException(){}
};


/**
 * @class InvalidDataTypeException
 * @brief Exception for invalid data type errors.
 *
 * Use this exception to indicate that a data type is invalid.
 */
class InvalidDataTypeException : public Exception
{
public:
    InvalidDataTypeException(const std::string &source, Exception* innerException) 
        : Exception("InvalidDataTypeException", source, "Invalid data type", ErrorCode::CRITICAL_ERROR_CODE, innerException) {}
    InvalidDataTypeException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("InvalidDataTypeException", "Unknown source", message, code) {}
    InvalidDataTypeException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("InvalidDataTypeException", source, message, code) {}
    InvalidDataTypeException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("InvalidDataTypeException", source, message, code, innerException) {}
    ~InvalidDataTypeException(){}
};

/**
 * @class InvalidDataFormatException
 * @brief Exception for invalid data format errors.
 *
 * Use this exception to indicate that data format is invalid or corrupted.
 */
class InvalidDataFormatException : public Exception
{
public:
    InvalidDataFormatException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    InvalidDataFormatException(const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(message, code) {}
    InvalidDataFormatException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(source, message, code) {}
    ~InvalidDataFormatException(){}
};

#endif // DATA_EXCEPTIONS_HPP

