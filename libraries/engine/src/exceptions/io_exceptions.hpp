#ifndef IO_EXCEPTIONS_HPP
#define IO_EXCEPTIONS_HPP

/**
 * @file io_exceptions.hpp
 * @brief Specialized exceptions for IO communication errors.
 *
 * This header defines exception classes for IO communication errors, such as failures during send or receive operations.
 * These exceptions inherit from the base Exception class and provide constructors for different error reporting scenarios.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#include "expt.hpp" ///< For Exception class

/**
 * @class IOCommSendException
 * @brief Exception for IO send operation failures.
 *
 * Use this exception to indicate errors during IO send operations.
 */
class IOCommSendException : public Exception
{
public:
    IOCommSendException(const std::string &source, Exception* innerException) 
        : Exception("IOCommSendException", source, "IO send operation failed", ErrorCode::ERROR_CODE, innerException) {}
    IOCommSendException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommSendException", "Unknown source", message, code) {}
    IOCommSendException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommSendException", source, message, code) {}
    IOCommSendException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommSendException", source, message, code, innerException) {}
    ~IOCommSendException(){}
};

/**
 * @class IOCommReceiveException
 * @brief Exception for IO receive operation failures.
 *
 * Use this exception to indicate errors during IO receive operations.
 */
class IOCommReceiveException : public Exception
{
public:
    IOCommReceiveException(const std::string &source, Exception* innerException) 
        : Exception("IOCommReceiveException", source, "IO receive operation failed", ErrorCode::ERROR_CODE, innerException) {}
    IOCommReceiveException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommReceiveException", "Unknown source", message, code) {}
    IOCommReceiveException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommReceiveException", source, message, code) {}
    IOCommReceiveException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("IOCommReceiveException", source, message, code, innerException) {}
    ~IOCommReceiveException(){}
};

/**
 * @class IOChannelNotInitializedException
 * @brief Exception for uninitialized IO channel errors.
 *
 * Use this exception to indicate that an IO channel was not initialized before use.
 */
class IOChannelNotInitializedException : public Exception
{
public:
    IOChannelNotInitializedException(const std::string &source, Exception* innerException) 
        : Exception("IOChannelNotInitializedException", source, "IO channel not initialized", ErrorCode::CRITICAL_ERROR_CODE, innerException) {}
    IOChannelNotInitializedException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelNotInitializedException", "Unknown source", message, code) {}
    IOChannelNotInitializedException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelNotInitializedException", source, message, code) {}
    IOChannelNotInitializedException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelNotInitializedException", source, message, code, innerException) {}
    ~IOChannelNotInitializedException(){}
};

/**
 * @class IOChannelInitializedErrorException
 * @brief Exception for already-initialized IO channel errors.
 *
 * Use this exception to indicate that an IO channel was already initialized when an initialization was attempted.
 */
class IOChannelInitializedErrorException : public Exception
{
public:
    IOChannelInitializedErrorException(const std::string &source, Exception* innerException) 
        : Exception("IOChannelInitializedErrorException", source, "IO channel already initialized", ErrorCode::CRITICAL_ERROR_CODE, innerException) {}
    IOChannelInitializedErrorException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelInitializedErrorException", "Unknown source", message, code) {}
    IOChannelInitializedErrorException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelInitializedErrorException", source, message, code) {}
    IOChannelInitializedErrorException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("IOChannelInitializedErrorException", source, message, code, innerException) {}
    ~IOChannelInitializedErrorException(){}
};

#endif // IO_EXCEPTIONS_HPP
