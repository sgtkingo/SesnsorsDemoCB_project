/**
 * @file error_codes.hpp
 * @brief Defines error codes used in exception handling and sensor operations.
 * 
 * This header provides a strongly typed enumeration for error codes that were previously defined
 * as preprocessor macros. These error codes are used throughout the application for configuration,
 * update, and value validation.
 * 
 * @copyright 2024 MTA
 * @author
 * Ing. Jiri Konecny
 */

#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

/**
 * @enum ErrorCode
 * @brief Enumeration of error codes for sensor and exception handling.
 *
 * - **VALUE_ERROR**: Indicates that a value is invalid.
 * - **VALUE_NOT_FOUND**: Indicates that a required value was not found.
 * - **CONFIG_ERROR**: Indicates an error in sensor configuration.
 * - **UPDATE_ERROR**: Indicates an error during an update operation.
 */
enum ErrorCode {
    VALUE_ERROR     = -1,  ///< Invalid value error.
    VALUE_NOT_FOUND =  0,  ///< Value not found.
    WARNING_CODE    =  1,  ///< Configuration error.
    ERROR_CODE    =  2,   ///< Update operation error.
    CRTICAL_ERROR_CODE   =  3,   ///< Unknown error.
    NOT_DEFINED_ERROR = 1000
};

#endif // ERROR_CODES_HPP
