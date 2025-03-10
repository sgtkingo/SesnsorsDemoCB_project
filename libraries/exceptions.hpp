/**
 * @file exceptions.hpp
 * @brief Declaration of the Exception class for error handling.
 * 
 * This header file declares the Exception class, which encapsulates an error code,
 * message, and source information. It also supports nested (chained) exceptions via
 * an innerException pointer. The class provides a method to recursively print exception
 * details with indentation.
 * 
 * @copyright 2024 MTA
 * @author 
 * Ing. Jiri Konecny
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

/*********************
 *      INCLUDES
 *********************/
#include "logs.hpp"   ///< For logMessage function
#include "error_codes.hpp"  ///< For error codes

#include <stdexcept>   ///< For std::exception

/**
 * @class Exception
 * @brief Represents an error/exception in the application.
 *
 * The Exception class stores an error code, a descriptive message, and the source where
 * the exception occurred. It also supports chaining of exceptions via the innerException
 * pointer. When an Exception object is destroyed, any chained exceptions are automatically
 * deleted.
 */
class Exception : public std::exception {
public:
    ErrorCode Code;               ///< Error code associated with the exception.
    std::string Message;    ///< Human-readable exception message.
    std::string Source;     ///< Origin of the exception (e.g., function or module name).
    Exception* innerException; ///< Pointer to a nested exception (if any).

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param ex Pointer to a std::exception object.
     */
    Exception(std::exception ex)
    {
        Code = ErrorCode::NOT_DEFINED_ERROR;
        Message = ex.what();
        Source = "std::exception";
        innerException = nullptr;
    }

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param source The source of the exception.
     * @param inner Pointer to a nested exception.
     */
    Exception(const std::string &source, Exception* inner)
    {
        Code = ErrorCode::NOT_DEFINED_ERROR;
        Message = "Unknown error";
        Source = source;
        innerException = inner;
    }

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param message The descriptive message.
     */
    Exception(const std::string &message, ErrorCode code = ErrorCode::NOT_DEFINED_ERROR)
    {
        Code = code;
        Message = message;
        Source = "Unknown source";
        innerException = nullptr;
    }

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param source The source of the exception.
     * @param message The descriptive message.
     * @param code (Optional) The error code.
     * @param inner (Optional) Pointer to a nested exception. Defaults to nullptr.
     */
    Exception(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::NOT_DEFINED_ERROR, Exception* inner = nullptr)
    : Code(code), Message(message), Source(source), innerException(inner) {}

    /**
     * @brief Destructor for the Exception object.
     * 
     * Automatically deletes any chained (inner) exception.
     */
    virtual ~Exception() { delete innerException; }

    /**
     * @brief Recursively prints the exception and any nested exceptions.
     * 
     * This function prints the exception message with indentation corresponding to the
     * nesting level. It then recursively prints any inner exceptions.
     * 
     * @param level The current indentation level (default is 0).
     */
    void print(int level = 0) const {
        for (int i = 0; i < level; i++) {
            logMessage(" \t");
        }
        logMessage("(%s) Exception catch: %s\n", Source.c_str(), Message.c_str());
        if (innerException) {
            innerException->print(level + 1);
        }
    }
};

class ConfigurationNotFoundException : public Exception
{
private:
    /* data */
public:
    ConfigurationNotFoundException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    ConfigurationNotFoundException(const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) : Exception(message, code) {};
    ConfigurationNotFoundException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) : Exception(source, message, code) {};
    ~ConfigurationNotFoundException(){};
};

class InvalidConfigurationException : public Exception
{
private:
    /* data */
public:
    InvalidConfigurationException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    InvalidConfigurationException(const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(message, code) {};
    InvalidConfigurationException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(source, message, code) {};
    ~InvalidConfigurationException(){};
};

class ValueNotFoundException : public Exception
{
private:
    /* data */
public:
    ValueNotFoundException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    ValueNotFoundException(const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) : Exception(message, code) {};
    ValueNotFoundException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::NOT_FOUND) : Exception(source, message, code) {};
    ~ValueNotFoundException(){};
};


class InvalidValueException : public Exception
{
private:
    /* data */
public:
    InvalidValueException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    InvalidValueException(const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(message, code) {};
    InvalidValueException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::INVALID_VALUE) : Exception(source, message, code) {};
    ~InvalidValueException(){};
};

class InvalidDataTypeException : public Exception
{
private:
    /* data */
public:
    InvalidDataTypeException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    InvalidDataTypeException(const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(message, code) {};
    InvalidDataTypeException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code) {};
    InvalidDataTypeException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code, innerException) {};
    ~InvalidDataTypeException(){};
};

class SensorInitializationFailException : public Exception
{
private:
    /* data */
public:
    SensorInitializationFailException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    SensorInitializationFailException(const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(message, code) {};
    SensorInitializationFailException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code) {};
    SensorInitializationFailException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code, innerException) {};
    ~SensorInitializationFailException(){};
};

class ParseMetadataException : public Exception
{
private:
    /* data */
public:
    ParseMetadataException(const std::string &source, Exception* innerException) : Exception(source, innerException) {};
    ParseMetadataException(const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(message, code) {};
    ParseMetadataException(const std::string &source, const std::string &message, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code) {};
    ParseMetadataException(const std::string &source, const std::string &message, Exception *innerException, ErrorCode code = ErrorCode::CRTICAL_ERROR_CODE) : Exception(source, message, code, innerException) {};
    ~ParseMetadataException(){};
};


#endif // EXCEPTIONS_HPP
