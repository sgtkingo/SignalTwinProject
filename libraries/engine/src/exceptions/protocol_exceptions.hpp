
/**
 * @file protocol_exceptions.hpp
 * @brief Specialized exceptions for protocol operations.
 *
 * This header defines exception classes for protocol-related errors, such as failures in protocol methods
 * and metadata parsing. These exceptions inherit from the base Exception class and provide constructors
 * for various error reporting scenarios.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef PROTOCOL_EXCEPTIONS_HPP
#define PROTOCOL_EXCEPTIONS_HPP

#include "expt.hpp" ///< For Exception class

/**
 * @class ParseMetadataException
 * @brief Exception for errors during protocol metadata parsing.
 *
 * Use this exception to indicate failures when parsing protocol metadata.
 */
class ParseMetadataException : public Exception
{
public:
    ParseMetadataException(const std::string &source, Exception* innerException) 
        : Exception("ParseMetadataException", source, "Protocol metadata parsing failed", ErrorCode::ERROR_CODE, innerException) {}
    ParseMetadataException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ParseMetadataException", "Unknown source", message, code) {}
    ParseMetadataException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ParseMetadataException", source, message, code) {}
    ParseMetadataException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ParseMetadataException", source, message, code, innerException) {}
    ~ParseMetadataException(){}
};


/**
 * @class ProtocolMethodFailException
 * @brief Exception for failures in protocol method execution.
 *
 * Use this exception to indicate errors that occur during protocol method calls.
 */
class ProtocolMethodFailException : public Exception
{
public:
    ProtocolMethodFailException(const std::string &source, Exception* innerException) 
        : Exception("ProtocolMethodFailException", source, "Protocol method execution failed", ErrorCode::ERROR_CODE, innerException) {}
    ProtocolMethodFailException(const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ProtocolMethodFailException", "Unknown source", message, code) {}
    ProtocolMethodFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ProtocolMethodFailException", source, message, code) {}
    ProtocolMethodFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::ERROR_CODE) 
        : Exception("ProtocolMethodFailException", source, message, code, innerException) {}
    ~ProtocolMethodFailException(){}
};


/**
 * @class ProtocolNotInitializedException
 * @brief Exception for protocol not initialized errors.
 *
 * Use this exception to indicate that the protocol has not been properly initialized
 * before attempting to use it.
 */
class ProtocolNotInitializedException : public Exception
{
public:
    ProtocolNotInitializedException(const std::string &source, Exception* innerException) 
        : Exception("ProtocolNotInitializedException", source, "Protocol not initialized", ErrorCode::CRITICAL_ERROR_CODE, innerException) {}
    ProtocolNotInitializedException(const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("ProtocolNotInitializedException", "Unknown source", message, code) {}
    ProtocolNotInitializedException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("ProtocolNotInitializedException", source, message, code) {}
    ProtocolNotInitializedException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRITICAL_ERROR_CODE) 
        : Exception("ProtocolNotInitializedException", source, message, code, innerException) {}
    ~ProtocolNotInitializedException(){}
};

#endif // PROTOCOL_EXCEPTIONS_HPP

