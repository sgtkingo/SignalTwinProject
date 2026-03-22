/**
 * @file devices_exceptions.hpp
 * @brief Specialized exceptions for device operations.
 *
 * This header defines exception classes for various device-related errors, such as initialization,
 * synchronization, reset, connection, and disconnection failures. These exceptions inherit from the base
 * Exception class and provide constructors for different error reporting scenarios.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef DEVICES_EXCEPTIONS_HPP
#define DEVICES_EXCEPTIONS_HPP

#include "expt.hpp"

/**
 * @class DeviceInitializationFailException
 * @brief Exception for device initialization failures.
 *
 * Use this exception to indicate errors during device initialization.
 */
class DeviceInitializationFailException : public Exception
{
public:
    DeviceInitializationFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DeviceInitializationFailException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(message, code) {}
    DeviceInitializationFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(source, message, code) {}
    DeviceInitializationFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DeviceInitializationFailException(){}
};


/**
 * @class DeviceSynchronizationFailException
 * @brief Exception for device synchronization failures.
 *
 * Use this exception to indicate errors during device synchronization.
 */
class DeviceSynchronizationFailException : public Exception
{
public:
    DeviceSynchronizationFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DeviceSynchronizationFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    DeviceSynchronizationFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    DeviceSynchronizationFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DeviceSynchronizationFailException(){}
};


/**
 * @class DeviceResetFailException
 * @brief Exception for device reset failures.
 *
 * Use this exception to indicate errors during device reset operations.
 */
class DeviceResetFailException : public Exception
{
public:
    DeviceResetFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DeviceResetFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    DeviceResetFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    DeviceResetFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DeviceResetFailException(){}
};

/**
 * @class DeviceDisconnectFailException
 * @brief Exception for device disconnection failures.
 *
 * Use this exception to indicate errors during device disconnection operations.
 */
class DeviceDisconnectFailException : public Exception
{
public:
    DeviceDisconnectFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DeviceDisconnectFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    DeviceDisconnectFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    DeviceDisconnectFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DeviceDisconnectFailException(){}
};


/**
 * @class DeviceConnectionFailException
 * @brief Exception for device connection failures (general connection errors).
 *
 * Use this exception to indicate errors when a device fails to establish or maintain a connection.
 */
class DeviceConnectionFailException : public Exception
{
public:
    DeviceConnectionFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DeviceConnectionFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    DeviceConnectionFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    DeviceConnectionFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DeviceConnectionFailException(){}
};


/**
 * @class DevicePinAssignmentException
 * @brief Exception for device pin assignment errors.
 *
 * Use this exception to indicate errors when assigning pins to devices (e.g., invalid, unavailable, or conflicting pin assignments).
 */
class DevicePinAssignmentException : public Exception
{
public:
    DevicePinAssignmentException(const std::string &source, Exception* innerException) : Exception(source, innerException) {}
    DevicePinAssignmentException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(message, code) {}
    DevicePinAssignmentException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code) {}
    DevicePinAssignmentException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) : Exception(source, message, code, innerException) {}
    ~DevicePinAssignmentException(){}
};

#endif // DEVICES_EXCEPTIONS_HPP
