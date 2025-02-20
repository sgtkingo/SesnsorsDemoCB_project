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


std::string getValueFromKeyValueLikeString(std::string str, std::string key) {
    std::string value;
    size_t pos = str.find(key);
    if(pos != std::string::npos) {
        pos += key.length() + 1;
        size_t end = str.find("&", pos);
        value = str.substr(pos, end - pos);
    }

    return value;
}


/*Create functions*/
//implemented in sensors classes...

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
