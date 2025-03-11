/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

#ifndef __MANAGER_H_
#define __MANAGER_H_

/*********************
 *      INCLUDES
 *********************/
#include "parser.hpp"
#include "messenger.hpp"
#include "sensors.hpp"

#include <vector>
#include <algorithm>

/**********************
 *      TYPEDEFS
 **********************/
 
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

    void init(){
        logMessage("Initializing manager to default state...\n");
        Sensors.clear();
    }

    void init(std::string response){
        logMessage("Initializing manager via sensors list...\n");
        Sensors.clear();

        //Check request format
        if(response.size() < 1 || response[0] != '?')
        {
            throw Exception("InitManager", "Invalid response format!");
        }
        else
        {
            //Get rid of the '?' character
            response.erase(0, 1);
        }

        //Add sensors here
        //Expected format: ?0:ADC&1:ADC&2:TH
        std::vector<std::string> sensorList = splitString(response, '&');
        logMessage("\t(i)Found %d sensors...\n", sensorList.size());
        std::string id;
        std::string type;

        for (std::string sensor: sensorList)
        {
            logMessage("\tProcessing sensor request: %s\n", sensor.c_str());
            if (sensor.empty())
            {
                continue;
            }
            id = sensor.substr(0, sensor.find(':'));
            type = sensor.substr(sensor.find(':') + 1);

            //For ADC
            if (type == "ADC")
            {
                addSensor(new ADC(id));
                logMessage("\t(*)Detected known sensor type:%s, sensor with ID:%s added!\n", type.c_str(), id.c_str());
            }
            //For TH
            else if (type == "TH")
            {
                addSensor(new TH(id));
                logMessage("\t(*)Detected known sensor type:%s, sensor with ID:%s added!\n", type.c_str(), id.c_str());
            }
            else
            {
                logMessage("\t(!)Unknown sensor type:%s, sensor with ID:%s added!\n", type.c_str(), id.c_str());
            }
        }
        
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

    void resync()
    {
        for (BaseSensor* sensor : Sensors)
        {
            syncSensor(sensor);
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