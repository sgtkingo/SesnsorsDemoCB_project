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
        init();
     };

    ~ SensorManager(){};

    void init(){
        logMessage("Initializing sensors...\n");
        Sensors.clear();
        Sensors.push_back(new ADC(0));
        Sensors.push_back(new TH(1));
    }

    BaseSensor* getSensor(int id)
    {
        for (BaseSensor* sensor : Sensors)
        {
            if(sensor->UID == id)
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

    void manage(std::string request)
    {
        //Check if request is empty to prevent unnecessary parsing
        if( request.empty() )
        {
            return;
        }

        //Parsing starts here
        SensorMetadata metadata;
        try
        {
            metadata = ParseMetadata(request);
        }
        catch(const Exception& e)
        {
            e.print();
            return;
        }

        //Check if metadata is valid
        if(!CheckMetadata(&metadata))
        {
            logMessage("Invalid metadata!\n");
            return;
        }

        //Get sensor
        BaseSensor* sensor = getSensor(metadata.ID);
        if(sensor == nullptr)
        {
            logMessage("Sensor with ID %d not found!\n", metadata.ID);
            return;
        }

        //Process request
        updateSensor(sensor, metadata.Data);
    }

    void print(int id)
    {
        BaseSensor* sensor = getSensor(id);
        printSensor(sensor);
    }

    void redraw()
    {
        for (BaseSensor* sensor : Sensors)
        {
            sensor->draw();
        }
    }

    void sync()
    {
        for (BaseSensor* sensor : Sensors)
        {
            sensor->synchronize();
        }
    }   
};


/*********************
 *      DECLARES
 *********************/


#endif //__MANAGER_H_