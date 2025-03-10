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

/*********************
 *      INCLUDES
 *********************/
#include "exceptions.hpp"  ///< Exceptions.
#include "helpers.hpp"     ///< Helper functions.

#include <string>
#include <unordered_map>
#include <map>

/**
 * @enum SensorStatus
 * @brief Enumeration representing possible sensor statuses.
 *
 * - OK: Sensor is operating normally.
 * - ERROR: Sensor has encountered an error.
 * - OFFLINE: Sensor is offline.
 */
enum class SensorStatus {
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
    std::string UID;                ///< Unique sensor identifier.
    SensorStatus Status;             ///< Sensor status.
    std::string Type;       ///< Sensor type as text.
    std::string Description;///< Description of the sensor.
    Exception *Error;       ///< Pointer to an exception object (if any).

    //lv_obj_t *ui_Container; ///< Pointer to the UI widgets container.

    std::unordered_map<std::string, SensorParam> Values; ///< Sensor values.
    std::map<std::string, SensorParam> Configs;          ///< Sensor configurations.

    bool redrawPenging = true;    ///< Flag to indicate if sensor needs to be redrawn.
    bool isSync = false;          ///< Flag to indicate if sensor is synchronized with real sensor.

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
    bool operator==(const std::string uid) const {
        return UID == uid;
    }

    /**
     * @brief Constructs a new BaseSensor object.
     * 
     * @param uid The unique sensor identifier.
     */
    BaseSensor(std::string uid) : UID(uid), Status(SensorStatus::OK) 
    {
        Error = nullptr;

        redrawPenging = true;
        isSync = false;
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseSensor() 
    {
        delete Error;
    }

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
     * @brief Set configuration value.
     * 
     * This function sets the value of a configuration parameter by key.
     * 
     * @param key The key of the configuration parameter.
     * @param value The value to set.
     */
    void setConfig(const std::string &key, const std::string &value) {
        if (Configs.find(key) != Configs.end()) {
            Configs[key].Value = value;
        }
        else{
            throw ConfigurationNotFoundException("BaseSensor::setConfig", "Configuration not found for key: " + key);
        }

        isSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
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
     * @brief Set sensor value.
     * 
     * This function sets the value of a sensor parameter by key.
     * 
     * @param key The key of the sensor parameter.
     * @param value The value to set.
     */
    void setValue(const std::string &key, const std::string &value) {
        if (Values.find(key) != Values.end()) {
            Values[key].Value = value;
        }
        else{
            throw ValueNotFoundException("BaseSensor::setValue", "Value not found for key: " + key);
        }
        
        isSync = false; // Set flag to indicate sensor is not synchronized with real sensor.
    }

    /**
     * @brief Get units of sensor value parameter.
     * 
     * This function retrieves the units of a sensor value parameter by key.
     * 
     * @param key The key of the value sensor parameter.
     * @return The units of the value sensor parameter.
     */
    std::string getValueUnits(const std::string &key) {
        if (Values.find(key) != Values.end()) {
            return Values[key].Unit;
        }
        return "";
    }

    /**
     * @brief Get units of sensor config parameter.
     * 
     * This function retrieves the units of a sensor config parameter by key.
     * 
     * @param key The key of the sensor config parameter.
     * @return The units of the sensor config parameter.
     */
    std::string getConfigUnits(const std::string &key) {
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
    std::string getBasicComHeader() const {
        return "?type=" + Type + "&id=" + UID;
    }

    /**
     * @brief Set exception as error and change status accordingly.
     * 
     * @param error The exception as error.
     */
    void setError(Exception *error) {
        if (Error) {
            delete Error;
            Status = SensorStatus::OK;
        }

        Error = error;
        if(Error)
        {
            if( Error->Code != ErrorCode::WARNING_CODE ) {
                Status = SensorStatus::ERROR;
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
     * @brief Add configuration parameter to the sensor.
     * 
     * @param key The key of the configuration parameter.
     * @param param The configuration parameter to add.
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
     * @brief Synchronize configuration with the real sensor.
     * 
     * @throws Exception if synchronization fails.
     */
    virtual void synchronize()
    {
        std::string syncMessage = getBasicComHeader();
        for (auto &c : Configs) {
            syncMessage += "&" + c.first + "=" + c.second.Value;
        }
        
        //Send config changes to real sensor
        //TODO: Implement sending config changes to real sensor
        logMessage("Sync message: %s\n", syncMessage.c_str());

        isSync = true;
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

        redrawPenging = true; // Set flag to redraw sensor - values updated.
        isSync = true; // Set flag to indicate sensor is synchronized with real sensor.
    }

    /**
     * @brief Adds a value parameter to the sensor.
     * 
     * @param key The key of the value parameter.
     * @param param The value parameter to add.
     * @throws Exception if adding the value parameter fails.
     */
    void addValueParameter(const std::string &key, const SensorParam &param) {
        try
        {
            Values[key] = param;
        }
        catch(const std::exception& e)
        {
            throw InvalidValueException("BaseSensor::addValueParameter", new Exception(e));
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
            logMessage("Sensor UID: %s\n", UID.c_str());
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

    /**
     * @brief Construct UI elements.
     * 
     * This function should be overridden by derived classes to construct sensor-specific GUI.
    */
    virtual void construct() = 0;
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
    ADC(std::string uid) : BaseSensor(uid)
    {
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

        //Call for construct UI elements
        construct();
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

    /**
     * @brief Construct UI elements.
     * 
     * This function constructs the sensor-specific GUI.
     */
    virtual void construct() override {
        // Construct sensor UI

        // Call construct LVGL functions here
    }
};


/**************************************************************************/

/**
 * @class TH
 * @brief Temperature/Huminidy sensor class derived from BaseSensor.
 * 
 * Represents a Temperature/Huminidy sensor. Implements initialization, configuration, updating, and printing
 * specific to Temperature/Huminidy sensors.
 */
class TH : public BaseSensor {
    public:
        /**
         * @brief Constructs a new TH object.
         * 
         * Initializes default values and sets the sensor type and description.
         * 
         * @param uid The unique sensor identifier.
         */
        TH(std::string uid) : BaseSensor(uid)
        {
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

            //Call for construct UI elements
            construct();
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

        /**
         * @brief Construct UI elements.
         * 
         * This function constructs the sensor-specific GUI.
         */
        virtual void construct() override {
            // Construct sensor UI

            // Call construct LVGL functions here
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
 * @throws SensorInitializationFailException if sensor initialization fails.
 */
template<typename T>
T* createSensor(std::string uid) {
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
 * @throws Exceptions should be internally resolved to prevent program from crash.
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
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
void updateSensor(BaseSensor *sensor, const std::string &update);

/**
 * @brief Prints detailed information about the sensor.
 * 
 * This function prints sensor details by calling the sensor's own printSensor() method,
 * which includes both basic sensor info and any additional sensor-specific data.
 * 
 * @param sensor Pointer to the sensor whose information is to be printed.
 * @throws Exceptions should be internally resolved to prevent program from crash.
 */
void printSensor(BaseSensor *sensor);



#endif //SENSORS_HPP
