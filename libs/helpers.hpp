/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

#ifndef __HELPERS_H_
#define __HELPERS_H_

/*********************
 *      INCLUDES
 *********************/
#include "exceptions.hpp"
#include <string>
#include <type_traits>      ///< For is_same

/**********************
 *      TYPEDEFS
 **********************/


/*********************
 *      DECLARES
 *********************/

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

#endif //__HELPERS_H_