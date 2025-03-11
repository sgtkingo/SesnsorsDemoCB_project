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

/*Global functions*/


/*Create functions*/
//implemented in header file as generic function

/*General functions*/

void configSensor(BaseSensor *sensor, const std::string &config) {
    if(sensor == nullptr) {
        return;
    }

    try {
        sensor->config(config);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}


void updateSensor(BaseSensor *sensor, const std::string &update) {
    if(sensor == nullptr) {
        return;
    }

    try {
        sensor->update(update);
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void printSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return;
    }
    
    try {
        sensor->print();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void syncSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return;
    }

    try {
        sensor->synchronize();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void drawSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return;
    }

    try {
        sensor->draw();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}

void constructSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return;
    }

    try {
        sensor->construct();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(new Exception(ex));
    }
}
