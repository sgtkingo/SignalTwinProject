/**
 * @file files_exceptions.hpp
 * @brief File-related exception classes for the Virtual Sensors project.
 * 
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef FILES_EXCEPTIONS_HPP
#define FILES_EXCEPTIONS_HPP

#include "expt.hpp"

/**
 * @brief Exception thrown when a file is not found or cannot be accessed.
 */
class FileNotFoundException : public Exception {
public:
    /**
     * @brief Constructor for FileNotFoundException.
     * @param method The method where the exception occurred
     * @param message The error message
     */
    FileNotFoundException(const std::string& method, const std::string& message)
        : Exception("FileNotFoundException", method, message) {}
};

/**
 * @brief Exception thrown when file reading operation fails.
 */
class FileReadException : public Exception {
public:
    /**
     * @brief Constructor for FileReadException.
     * @param method The method where the exception occurred
     * @param message The error message
     */
    FileReadException(const std::string& method, const std::string& message)
        : Exception("FileReadException", method, message) {}
};

/**
 * @brief Exception thrown when file writing operation fails.
 */
class FileWriteException : public Exception {
public:
    /**
     * @brief Constructor for FileWriteException.
     * @param method The method where the exception occurred
     * @param message The error message
     */
    FileWriteException(const std::string& method, const std::string& message)
        : Exception("FileWriteException", method, message) {}
};

/**
 * @brief Exception thrown when file format is invalid or corrupted.
 */
class InvalidFileFormatException : public Exception {
public:
    /**
     * @brief Constructor for InvalidFileFormatException.
     * @param method The method where the exception occurred
     * @param message The error message
     */
    InvalidFileFormatException(const std::string& method, const std::string& message)
        : Exception("InvalidFileFormatException", method, message) {}
};

#endif // FILES_EXCEPTIONS_HPP