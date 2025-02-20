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
    int Code;               ///< Error code associated with the exception.
    std::string Message;    ///< Human-readable exception message.
    std::string Source;     ///< Origin of the exception (e.g., function or module name).
    Exception* innerException; ///< Pointer to a nested exception (if any).

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param source The source of the exception.
     * @param message The descriptive message.
     * @param code (Optional) The error code.
     * @param inner (Optional) Pointer to a nested exception. Defaults to nullptr.
     */
    Exception(const std::string &source, const std::string &message, int code = NOT_DEFINED_ERROR, Exception* inner = nullptr)
    : Code(code), Message(message), Source(source), innerException(inner) {}

    /**
     * @brief Constructs a new Exception object.
     * 
     * @param message The descriptive message.
     * @param code (Optional) The error code.
     * @param inner (Optional) Pointer to a nested exception. Defaults to nullptr.
     */
    Exception(const std::string &message, int code = NOT_DEFINED_ERROR, Exception* inner = nullptr)
    : Exception("non-defined source", message, code, inner) {}

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
    ConfigurationNotFoundException(const std::string &message, int code = WARNING_CODE) : Exception(message, code) {};
    ConfigurationNotFoundException(const std::string &source, const std::string &message, int code = WARNING_CODE) : Exception(source, message, code) {};
    ~ConfigurationNotFoundException(){};
};

class InvalidConfigurationException : public Exception
{
private:
    /* data */
public:
    InvalidConfigurationException(const std::string &message, int code = ERROR_CODE) : Exception(message, code) {};
    InvalidConfigurationException(const std::string &source, const std::string &message, int code = ERROR_CODE) : Exception(source, message, code) {};
    ~InvalidConfigurationException(){};
};

class ValueNotFoundException : public Exception
{
private:
    /* data */
public:
    ValueNotFoundException(const std::string &message, int code = WARNING_CODE) : Exception(message, code) {};
    ValueNotFoundException(const std::string &source, const std::string &message, int code = WARNING_CODE) : Exception(source, message, code) {};
    ~ValueNotFoundException(){};
};


class InvalidValueException : public Exception
{
private:
    /* data */
public:
    InvalidValueException(const std::string &message, int code = ERROR_CODE) : Exception(message, code) {};
    InvalidValueException(const std::string &source, const std::string &message, int code = ERROR_CODE) : Exception(source, message, code) {};
    ~InvalidValueException(){};
};


#endif // EXCEPTIONS_HPP
