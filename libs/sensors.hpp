/**
 * @file sensors.hpp
 * @brief Declaration and implementation of sensor classes for sensor management using built-in exceptions.
 * 
 * This header defines the abstract BaseSensor class and its derived ADC and CustomSensor classes.
 * BaseSensor provides a helper method printBasicInfo() to output common sensor details.
 * Derived classes override printSensor() to print extra sensor-specific information.
 * Factory functions are provided to create sensor instances.
 * 
 * @copyright 2024 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#ifndef SENSORS_HPP
#define SENSORS_HPP

#include "logs.hpp"         ///< Logging functions.
#include "exceptions.hpp"  ///< Exceptions.

#include <string>
#include <cstdio>           ///< For sscanf

/**
 * @enum SensorStatus
 * @brief Enumeration representing possible sensor statuses.
 *
 * - OK: Sensor is operating normally.
 * - ERROR: Sensor has encountered an error.
 * - OFFLINE: Sensor is offline.
 */
enum SensorStatus {
    OK = 0,       ///< Sensor operating normally.
    ERROR = -1,   ///< Sensor has an error.
    OFFLINE = 1   ///< Sensor is offline.
};

/**
 * @class BaseSensor
 * @brief Abstract base class for sensors.
 * 
 * Defines common properties and virtual methods for sensor initialization, configuration,
 * updating, and printing. It also provides a helper method, printBasicInfo(), that prints
 * common sensor details.
 */
class BaseSensor {
public:
    int UID;                ///< Unique sensor identifier.
    int Status;             ///< Sensor status.
    std::string Type;       ///< Sensor type as text.
    std::string Description;///< Description of the sensor.
    Exception *Error;       ///< Pointer to an exception object (if any).

    /**
     * @brief Constructs a new BaseSensor object.
     * 
     * @param uid The unique sensor identifier.
     */
    BaseSensor(int uid) : UID(uid), Status(OK) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseSensor() {}

    /**
     * @brief Prints basic sensor information.
     */
    void printBasicInfo() const {
        logMessage("Sensor UID: %d\n", UID);
        logMessage("\tSensor Type: %s\n", Type.c_str());
        logMessage("\tSensor Description: %s\n", Description.c_str());
        logMessage("\tSensor Status: %d\n", Status);
        if (Error) {
            logMessage("\tSensor Error: %s\n", Error->Message.c_str());
        }
        else{
            logMessage("\tSensor Error: None\n");
        }
    }

        /**
     * @brief Set exception as error and change status accordingly.
     * 
     * @param error The exception as error.
     */
    void setError(Exception *error) {
        if (Error) {
            delete Error;
            Status = OK;
        }

        Error = error;
        if(Error)
        {
            if( Error->Code != WARNING_CODE ) {
                Status = ERROR;
            }
        }
    }

    /**
     * @brief Initializes the sensor.
     * 
     * @throws std::runtime_error if initialization fails.
     */
    virtual void initSensor() = 0;

    /**
     * @brief Configures the sensor based on a configuration string.
     * 
     * @param config The configuration string.
     * @throws Exception if configuration fails.
     */
    virtual void configSensor(const std::string &config) = 0;

    /**
     * @brief Updates the sensor with new data.
     * 
     * @param update The update string containing new sensor data.
     * @throws Exception if update fails.
     */
    virtual void updateSensor(const std::string &update) = 0;

    /**
     * @brief Prints sensor information.
     * 
     * This function should be overridden by derived classes to print sensor-specific details.
     * @throws Exception if printing fails.
     */
    virtual void printSensor() = 0;
};

/**************************************************************************/
// SENSORS
/**************************************************************************/

/**
 * @class ADC
 * @brief ADC sensor class derived from BaseSensor.
 * 
 * Represents an Analog-to-Digital Converter (ADC) sensor. Implements initialization, configuration,
 * updating, and printing specific to ADC sensors.
 */
class ADC : public BaseSensor {
public:
    float Value;      ///< Measured ADC value.
    int Resolution;   ///< ADC resolution (e.g. 8, 10, or 12).

    /**
     * @brief Constructs a new ADC object.
     * 
     * Initializes default values and sets the sensor type and description.
     * 
     * @param uid The unique sensor identifier.
     */
    ADC(int uid) : BaseSensor(uid), Value(0.0f), Resolution(12) {
        initSensor();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~ADC() {}

    /**
     * @brief Initializes the ADC sensor.
     * 
     * Additional ADC initialization code can be added here.
     * 
     * @throws std::runtime_error if initialization fails.
     */
    virtual void initSensor() override {
        // Additional initialization for ADC can be added here.
        Type = "ADC";
        Description = "Analog to Digital Converter";
        Error = nullptr;
    }

    /**
     * @brief Configures the ADC sensor using a configuration string.
     * 
     * Expected format: "Resolution=<value>".
     * 
     * @param config The configuration string.
     * @throws Exception if configuration is missing or invalid.
     */
    virtual void configSensor(const std::string &config) override {
        int resolution = VALUE_ERROR;
        int scanStatus = sscanf(config.c_str(), "Resolution=%d", &resolution);
        if (scanStatus == 1) {
            if (resolution != 8 && resolution != 10 && resolution != 12) {
                throw InvalidConfigurationException("configADC", "Resolution must be 8, 10 or 12!");
            }
        }

        //Clear error
        setError(nullptr);
        Resolution = resolution;
    }

    /**
     * @brief Updates the ADC sensor with new measurement data.
     * 
     * Expected format: "Value=<value>".
     * 
     * @param update The update string.
     * @throws InvalidValueException if value is missing or invalid.
     */
    virtual void updateSensor(const std::string &update) override {
        float value = VALUE_ERROR;
        int scanStatus = sscanf(update.c_str(), "Value=%f", &value);
        if (scanStatus == 1) {
            if (value == static_cast<float>(VALUE_ERROR) && value < 0) {
                throw InvalidValueException("updateADC", "Value is not valid!");
            }
        }

        //Clear error
        setError(nullptr);
        Value = value;
    }

    /**
     * @brief Prints the ADC sensor's information.
     * 
     * First prints basic sensor info using BaseSensor::printBasicInfo(), then prints ADC-specific details.
     * 
     */
    virtual void printSensor() override {
        printBasicInfo();
        logMessage("\tValue: %.2f\n", Value);
        logMessage("\tResolution: %d\n", Resolution);
    }
};

/**************************************************************************/
// CREATE FUNCTIONS
/**************************************************************************/

 /**
  * @brief Factory function to create an ADC sensor instance.
  * 
  * Dynamically allocates an ADC sensor, calls initSensor(), and returns the pointer.
  * 
  * @param uid The unique sensor identifier.
  * @return Pointer to the newly created ADC sensor.
  * @throws std::runtime_error if initialization fails.
  */
 ADC* createADC(int uid);

 /**************************************************************************/
// GENERAL FUNCTIONS
/**************************************************************************/

/**
 * @brief Configures the sensor using the given configuration string.
 * 
 * This function applies the configuration specified in the string to the provided sensor.
 * The configuration string should follow the expected format for that sensor (e.g. "Resolution=12").
 * 
 * @param sensor Pointer to the sensor to be configured.
 * @param config The configuration string.
 * @throws Exception if the configuration fails.
 */
void configSensor(BaseSensor *sensor, const std::string &config);

/**
 * @brief Updates the sensor with new measurement data.
 * 
 * This function updates the sensor's parameters based on the provided update string.
 * The update string should follow the expected format (e.g. "Value=3.3").
 * 
 * @param sensor Pointer to the sensor to be updated.
 * @param update The update string containing new sensor data.
 * @throws Exception if the update fails.
 */
void updateSensor(BaseSensor *sensor, const std::string &update);

/**
 * @brief Prints detailed information about the sensor.
 * 
 * This function prints sensor details by calling the sensor's own printSensor() method,
 * which includes both basic sensor info and any additional sensor-specific data.
 * 
 * @param sensor Pointer to the sensor whose information is to be printed.
 * @throws Exception if printing fails.
 */
void printSensor(BaseSensor *sensor);

#endif // SENSORS_HPP
