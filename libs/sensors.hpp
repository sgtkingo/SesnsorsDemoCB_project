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
#include <type_traits>      ///< For is_same
#include <unordered_map>
#include <map>

/**
 * @brief Get value from update string.
 * 
 * This function extracts the value of a given key from an update string.
 * 
 * @param update The update string.
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
T convertStringToType(const std::string &str) {
    if (str.empty()) {
        return T();
    }
    
    if constexpr (std::is_same_v<T, int>) {
        return std::stoi(str);
    } else if constexpr (std::is_same_v<T, double>) {
        return std::stod(str);
    } else if constexpr (std::is_same_v<T, float>) {
        return std::stof(str);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return str;
    } else {
        throw std::invalid_argument("Unsupported type conversion");
    }
}


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
 * @enum DataType
 * @brief Enumeration representing possible parametrs data types.
 *
 * - INT: int.
 * - DOUBLE: double.
*  - FLOAT: float.
 * - STRING: string.
 */
enum class DataType {
    INT,
    DOUBLE,
    FLOAT,
    STRING
};

/**
 * @struct SensorParam
 * @brief Structure for sensor parameters.
 * 
 * This structure can be used to store sensor parameters for configuration and updating.
 */
struct SensorParam
{
    std::string Value;  ///< Parameter value.
    std::string Unit;   ///< Parameter unit.
    DataType DataType; ///< Parameter data type.
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

    std::unordered_map<std::string, SensorParam> Values; ///< Sensor values.
    std::map<std::string, SensorParam> Configs;          ///< Sensor configurations.

    bool redrawPenging = true;    ///< Flag to indicate if sensor needs to be redrawn.

    /**
     * @brief Equality operator for comparing sensors by UID.
     * 
     * @param sensor The sensor to compare with.
     * @return true if the sensors have the same UID, false otherwise.
     */
    bool operator==(const BaseSensor &sensor) const {
        return UID == sensor.UID;
    }

    /**
     * @brief Equality operator for comparing sensors by UID.
     * 
     * @param uid The UID to compare with.
     * @return true if the sensor's UID matches the given UID, false otherwise.
     */
    bool operator==(const int uid) const {
        return UID == uid;
    }

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
     * @brief Get value from configuration.
     * 
     * This function retrieves the value of a configuration parameter by key.
     * 
     * @param key The key of the configuration parameter.
     * @return The value of the configuration parameter.
     */
    template <typename T>
    T getConfig(const std::string &key) {
        std::string value;
        if (Configs.find(key) != Configs.end()) {
            value = Configs[key].Value;
        }
        if(value.empty()) {
            throw ConfigurationNotFoundException("BaseSensor::getConfig", "Configuration not found for key: " + key);
        }
        
        try
        {
            return convertStringToType<T>(value);
        }
        catch(const std::exception& e)
        {
            throw InvalidDataTypeException("BaseSensor::getConfig", e.what());
        }       
    }

    /**
     * @brief Get value from sensor.
     * 
     * This function retrieves the value of a sensor parameter by key.
     * 
     * @param key The key of the sensor parameter.
     * @return The value of the sensor parameter.
     */
    template <typename T>
    T getValue(const std::string &key) {
        std::string value;
        if (Values.find(key) != Values.end()) {
            value = Values[key].Value;
        }
        if(value.empty()) {
            throw ValueNotFoundException("BaseSensor::getValue", "Value not found for key: " + key);
        }
        
        try
        {
            return convertStringToType<T>(value);
        }
        catch(const std::exception& e)
        {
            throw InvalidDataTypeException("BaseSensor::getValue", e.what());
        } 
    }

    /**
     * @brief Get units of sensor parameter.
     * 
     * This function retrieves the units of a sensor parameter by key.
     * 
     * @param key The key of the sensor parameter.
     * @return The units of the sensor parameter.
     */
    std::string getUnits(const std::string &key) {
        if (Values.find(key) != Values.end()) {
            return Values[key].Unit;
        }
        if (Configs.find(key) != Configs.end()) {
            return Configs[key].Unit;
        }
        return "";
    }

    /**
     * @brief Get basic communication header.
     * 
     * @return  The basic communication header.
     */
    std::string getBasicComHeader() {
        return "?type=" + Type + "&id=" + std::to_string(UID);
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
     * @brief Get error message.
     * 
     * @return The error message.
     */
    std::string getError() const {
        if(Error) {
            return Error->Message;
        }
        return "No error";
    }

    /**
     * @brief Configures the sensor with the given configuration string.
     * 
     * @param config The configuration string.
     * @throws Exception if configuration fails.
     */
    void addConfigParameter(const std::string &key, const SensorParam &param) {
        try
        {
            Configs[key] = param;
        }
        catch(const std::exception& e)
        {
            throw InvalidConfigurationException("BaseSensor::addConfigParameter", e.what());
        }
    }

    /**
     * @brief Get configures of the sensor as configuration string.
     * 
     * @throws Exception if configuration fails.
     * @return The configuration string.
     */
    virtual void config()
    {
        std::string config = getBasicComHeader();
        for (auto &c : Configs) {
            config += "&" + c.first + "=" + c.second.Value;
        }
        
        //Send config changes to real sensor
        //TODO: Implement sending config changes to real sensor
    }

    /**
     * @brief Configures the sensor with the given configuration string.
     * 
     * @param config The configuration string.
     * @throws Exception if configuration fails.
     */
    virtual void config(const std::string &cfg)
    {
        std::string value;
        // Parse the config string and update the sensor configs.
        for (auto &c : Configs) {
            value = getValueFromKeyValueLikeString(cfg, c.first, '&');
            if(!value.empty()) {
                c.second.Value = value;
            }
        }
        if(value.empty()) {
            throw ConfigurationNotFoundException("BaseSensor::config", "No configuration found in config string!");
        }

        config();
        redrawPenging = true; // Set flag to redraw sensor - values updated.
    }

    /**
     * @brief Update sensor with new data.
     * 
     * @param update The update string containing new sensor data.
     * @throws Exception if update fails.
     */
    void addValueParameter(const std::string &key, const SensorParam &param) {
        try
        {
            Values[key] = param;
        }
        catch(const std::exception& e)
        {
            throw InvalidValueException("BaseSensor::addValueParameter", e.what());
        }
    }

    /**
     * @brief Updates the sensor with new data.
     * 
     * @param update The update string containing new sensor data.
     * @throws Exception if update fails.
     */
    virtual void update(const std::string &upd)
    {
        std::string value;
        // Parse the update string and update the sensor values.
        for (auto &c : Values) {
            value = getValueFromKeyValueLikeString(upd, c.first, '&');
            if(!value.empty()) {
                c.second.Value = value;
            }
        }

        if(value.empty()) {
            throw ValueNotFoundException("BaseSensor::update", "No value found in update string!");
        }

        redrawPenging = true; // Set flag to redraw sensor - values updated.
    }

    /**
     * @brief Prints sensor information.
     * @throws Exception if print fails.
     */
    void print() const {
        try
        {
            logMessage("Sensor UID: %d\n", UID);
            logMessage("\tSensor Type: %s\n", Type.c_str());
            logMessage("\tSensor Description: %s\n", Description.c_str());
            logMessage("\tSensor Status: %d\n", Status);
            logMessage("\tSensor Error: %s\n", getError().c_str());
            logMessage("\tSensor Configurations:\n");
            for (auto &c : Configs) {
                logMessage("\t\t%s: %s %s\n", c.first.c_str(), c.second.Value.c_str(), c.second.Unit.c_str());
            }
            logMessage("\tSensor Values:\n");
            for (auto &v : Values) {
                logMessage("\t\t%s: %s %s\n", v.first.c_str(), v.second.Value.c_str(), v.second.Unit.c_str());
            }
        }
        catch(const std::exception& e)
        {
            throw;
        }
    }

    /**
     * @brief Initializes the sensor.
     * 
     * @throws Exception if initialization fails.
     */
    virtual void init() = 0;

    /**
     * @brief Draw sensor.
     * 
     * This function should be overridden by derived classes to draw sensor-specific details.
     */
    virtual void draw() = 0;
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
    /**
     * @brief Constructs a new ADC object.
     * 
     * Initializes default values and sets the sensor type and description.
     * 
     * @param uid The unique sensor identifier.
     */
    ADC(int uid) : BaseSensor(uid){
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~ADC() {}

    /**
     * @brief Initializes the sensor.
     * 
     * Additional initialization code can be added here.
     * 
     * @throws Exception if initialization fails.
     */
    virtual void init() override {
        // Additional initialization for sensor can be added here.
        Type = "ADC";
        Description = "Analog to Digital Converter";
        Error = nullptr;

        try
        {
            // Default configs
            addConfigParameter("Resolution", {"12", "bits", DataType::INT});
            // Default values
            addValueParameter("Value", {"0", "", DataType::INT});
        }
        catch(const std::exception& e)
        {
            throw;
        }
    }

    /**
     * @brief Draw sensor.
     * 
     * This function draws the sensor.
     */
    virtual void draw() override {
        if (!redrawPenging)
        {
            return;
        }
        
        // Draw sensor

        // Call draw function here
        //TODO: Implement draw function

        
        redrawPenging = false; // Reset flag to redraw sensor.
    }
};


/**************************************************************************/

class TH : public BaseSensor {
    public:
        /**
         * @brief Constructs a new TH object.
         * 
         * Initializes default values and sets the sensor type and description.
         * 
         * @param uid The unique sensor identifier.
         */
        TH(int uid) : BaseSensor(uid){
            init();
        }
    
        /**
         * @brief Virtual destructor.
         */
        virtual ~TH() {}

    
        /**
         * @brief Initializes the sensor.
         * 
         * Additional initialization code can be added here.
         * 
         * @throws Exception if initialization fails.
         */
        virtual void init() override {
            // Additional initialization for sensor can be added here.
            Type = "TH";
            Description = "Temperature & Humidity Sensor";
            Error = nullptr;
    
            try
            {
                // Default configs
                addConfigParameter("Precision", {"2", "decimals", DataType::INT});
                // Default values
                addValueParameter("Temperature", {"0", "Celsia", DataType::FLOAT});
                addValueParameter("Humidity", {"0", "%", DataType::INT});
            }
            catch(const std::exception& e)
            {
                throw;
            }
        }
    
        /**
         * @brief Draw sensor.
         * 
         * This function draws the sensor.
         */
        virtual void draw() override {
            if (!redrawPenging)
            {
                return;
            }
            // Draw sensor

            // Call draw function here
            //TODO: Implement draw function

            
            redrawPenging = false; // Reset flag to redraw sensor.
        }
    };


/**************************************************************************/
// CREATE FUNCTIONS
/**************************************************************************/

/**
 * @brief Factory function template to create a sensor of type T.
 * 
 * This function creates a sensor object of type T (which must have a constructor taking an int)
 * and returns a pointer to the newly created object. If initialization fails, it logs the error,
 * deletes the partially constructed object, and rethrows the exception.
 * 
 * @tparam T The sensor type, which must be derived from BaseSensor.
 * @param uid The unique sensor identifier.
 * @return T* Pointer to the newly created sensor.
 * @throws std::exception if sensor initialization fails.
 */
template<typename T>
T* createSensor(int uid) {
    static_assert(std::is_base_of<BaseSensor, T>::value, "T must be derived from BaseSensor");
    
    T* sensor = nullptr;
    try {
        sensor = new T(uid);
    } catch (const std::exception &ex) {
        logMessage("Error during sensor initialization: %s\n", ex.what());
        delete sensor;
        throw SensorInitializationFailException("createSensor", "Error during sensor initialization.", new Exception(ex));
    }

    logMessage("Sensor [%d]:%s created successfully.\n", sensor->UID, sensor->Type.c_str());
    return sensor;
}

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

#endif //SENSORS_HPP
