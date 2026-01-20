/**
 * @file sensors_exceptions.hpp
 * @brief Specialized exceptions for sensor operations.
 *
 * This header defines exception classes for various sensor-related errors, such as initialization,
 * synchronization, reset, connection, and disconnection failures. These exceptions inherit from the base
 * Exception class and provide constructors for different error reporting scenarios.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef SENSORS_EXCEPTIONS_HPP
#define SENSORS_EXCEPTIONS_HPP

#include "expt.hpp"

/**
 * @class SensorInitializationFailException
 * @brief Exception for sensor initialization failures.
 *
 * Use this exception to indicate errors during sensor initialization.
 */
class SensorInitializationFailException : public Exception
{
public:
    SensorInitializationFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorInitializationFailException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(message, code) {}
    SensorInitializationFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(source, message, code) {}
    SensorInitializationFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorInitializationFailException(){}
};


/**
 * @class SensorSynchronizationFailException
 * @brief Exception for sensor synchronization failures.
 *
 * Use this exception to indicate errors during sensor synchronization.
 */
class SensorSynchronizationFailException : public Exception
{
public:
    SensorSynchronizationFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorSynchronizationFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    SensorSynchronizationFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    SensorSynchronizationFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorSynchronizationFailException(){}
};


/**
 * @class SensorResetFailException
 * @brief Exception for sensor reset failures.
 *
 * Use this exception to indicate errors during sensor reset operations.
 */
class SensorResetFailException : public Exception
{
public:
    SensorResetFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorResetFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    SensorResetFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    SensorResetFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorResetFailException(){}
};

/**
 * @class SensorDisconnectFailException
 * @brief Exception for sensor disconnection failures.
 *
 * Use this exception to indicate errors during sensor disconnection operations.
 */
class SensorDisconnectFailException : public Exception
{
public:
    SensorDisconnectFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorDisconnectFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    SensorDisconnectFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    SensorDisconnectFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorDisconnectFailException(){}
};


/**
 * @class SensorConnectionFailException
 * @brief Exception for sensor connection failures (general connection errors).
 *
 * Use this exception to indicate errors when a sensor fails to establish or maintain a connection.
 */
class SensorConnectionFailException : public Exception
{
public:
    SensorConnectionFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorConnectionFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    SensorConnectionFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    SensorConnectionFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorConnectionFailException(){}
};


/**
 * @class SensorPinAssignmentException
 * @brief Exception for sensor pin assignment errors.
 *
 * Use this exception to indicate errors when assigning pins to sensors (e.g., invalid, unavailable, or conflicting pin assignments).
 */
class SensorPinAssignmentException : public Exception
{
public:
    SensorPinAssignmentException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    SensorPinAssignmentException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    SensorPinAssignmentException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    SensorPinAssignmentException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~SensorPinAssignmentException(){}
};

#endif // SENSORS_EXCEPTIONS_HPP