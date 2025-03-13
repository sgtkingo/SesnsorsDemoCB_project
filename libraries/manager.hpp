/**
 * @file manager.hpp
 * @brief Declaration of the manager 
 * 
 * This header defines the manager class for managing sensors.
 * 
 * @copyright 2025 MTA
 * @author 
 * Ing. Jiri Konecny
 */
#ifndef __MANAGER_H_
#define __MANAGER_H_

/*********************
 *      INCLUDES
 *********************/
#include "sensor_factory.hpp"
#include "messenger.hpp"
#include "parser.hpp"
#include "helpers.hpp"

#include <vector>
#include <algorithm>

/**********************
 *      TYPEDEFS
 **********************/

/*

*/
class SensorManager
{
private:
    std::vector<BaseSensor*> Sensors;
public:
    SensorManager(/* args */)
    {
        Sensors = std::vector<BaseSensor*>();
    };

    ~ SensorManager(){};

    void init(bool fromRequest = false){
        initMessenger();

        if(!fromRequest)
        {
            logMessage("Initializing manager via fixed sensors list...\n");
            createSensorList(Sensors);
            return;
        }

        //else
        logMessage("Initializing manager via request...\n");

        std::string request = "?INIT";
        sendMessage(request);
        std::string response = receiveMessage();
        //Check request format
        if(response.size() < 1 || response[0] != '?')
        {
            logMessage("Invalid sensor list format format!\n");
            init(false);
            return;
        }
        //Get rid of the '?' character
        response.erase(0, 1);

        createSensorList(Sensors, response); 
    }

    BaseSensor* getSensor(std::string uid)
    {
        for (BaseSensor* sensor : Sensors)
        {
            if(sensor->UID == uid)
            {
                return sensor;
            }
        }
        return nullptr;
    }

    void addSensor(BaseSensor* sensor)
    {
        if(sensor == nullptr)
        {
            return;
        }

        Sensors.push_back(sensor);
    }

    void sync(std::string id)
    {
        BaseSensor* sensor = getSensor(id);
        if(sensor != nullptr)
        {
            syncSensor(sensor);
        }
    }

    void print(std::string uid)
    {
        BaseSensor* sensor = getSensor(uid);
        printSensor(sensor);
    }

    void print()
    {
        for (BaseSensor* sensor : Sensors)
        {
            printSensor(sensor);
        }
    }

    void redraw()
    {
        for (BaseSensor* sensor : Sensors)
        {
            drawSensor(sensor);
        }
    }

    void reconstruct()
    {
        for (BaseSensor* sensor : Sensors)
        {
            constructSensor(sensor);
        }   
    }

    //Batch multi-update
    void resync()
    {
        /*
        for (BaseSensor* sensor : Sensors)
        {
            syncSensor(sensor);
        }
        */
       std::string request = "?UPDATE";
       sendMessage(request);

       std::string response = receiveMessage();
       std::vector<std::string> responses = splitString(response, '?');
       for(std::string resp : responses)
       {
           SensorMetadata metadata = ParseMetadata(resp);
           if(CheckMetadata(&metadata))
           {
               BaseSensor* sensor = getSensor(metadata.UID);
               if(sensor != nullptr)
               {
                   updateSensor(sensor, metadata.Data);
                   printSensor(sensor);
               }
           }
       }
    } 
    
    void erase()
    {
        for (BaseSensor* sensor : Sensors)
        {
            delete sensor;
        }
        Sensors.clear();
    }
};


/*********************
 *      DECLARES
 *********************/


#endif //__MANAGER_H_