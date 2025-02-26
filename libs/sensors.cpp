/**
 * @file sensors.cpp
 * @brief Implementation of sensor management functions.
 * 
 * This file implements sensor functionalities, including the factory function for creating
 * sensors instances. The factory function dynamically allocates an sensor, calls its
 * initialization method, and returns a pointer. If initialization fails, it logs the error,
 * cleans up, and rethrows the exception.
 * 
 * @copyright 2024 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#include "sensors.hpp"
#include <stdexcept>

/*Global functions*/

std::string getValueFromKeyValueLikeString(std::string str, std::string key, char separator = '&') {
    std::string value;
    size_t pos = str.find(key);
    if(pos != std::string::npos) {
        pos += key.length() + 1;
        size_t end = str.find(separator, pos);
        value = str.substr(pos, end - pos);
    }

    return value;
}

template <typename T>
T convertStringToType(const std::string &str) {
    throw std::invalid_argument("Unsupported type conversion");
}


// Specialization for int
template <>
int convertStringToType<int>(const std::string &str) {
    if (str.empty()) {
        return int(); // Return default-constructed int (0)
    }
    return std::stoi(str);
}

// Specialization for double
template <>
double convertStringToType<double>(const std::string &str) {
    if (str.empty()) {
        return double(); // Return default double (0.0)
    }
    return std::stod(str);
}

// Specialization for float
template <>
float convertStringToType<float>(const std::string &str) {
    if (str.empty()) {
        return float(); // Return default float (0.0f)
    }
    return std::stof(str);
}

// Specialization for std::string
template <>
std::string convertStringToType<std::string>(const std::string &str) {
    return str;
}



/*Create functions*/
//implemented in header file as generic function

/*General functions*/

void configSensor(BaseSensor *sensor, const std::string &config) {
    try {
        sensor->config(config);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}


void updateSensor(BaseSensor *sensor, const std::string &update) {
    try {
        sensor->update(update);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void printSensor(BaseSensor *sensor) {
    try {
        sensor->print();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}
