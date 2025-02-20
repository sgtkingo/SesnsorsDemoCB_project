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

ADC* createADC(int uid) {
    ADC* adc = nullptr;
    try {
        adc = new ADC(uid);
    } catch (const std::exception &ex) {
        logMessage("Error during ADC initialization: %s\n", ex.what());
        delete adc;
        throw;
    }
    return adc;
}

void configSensor(BaseSensor *sensor, const std::string &config) {
    try {
        sensor->configSensor(config);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void updateSensor(BaseSensor *sensor, const std::string &update) {
    try {
        sensor->updateSensor(update);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void printSensor(BaseSensor *sensor) {
    try {
        sensor->printSensor();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}
