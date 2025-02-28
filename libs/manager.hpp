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

    void init(std::string request){
        logMessage("Initializing manager via sensors list...\n");
        Sensors.clear();

        //Check request format
        if(request.size() < 1 || request[0] != '?')
        {
            throw Exception("InitManager", "Invalid request format!");
        }
        else
        {
            //Get rid of the '?' character
            request.erase(0, 1);
        }

        //Add sensors here
        //Expected format: ?0:ADC&1:ADC&2:TH
        std::vector<std::string> sensorRequestList = splitString(request, '&');
        logMessage("\t(i)Found %d sensors...\n", sensorRequestList.size());
        std::string id;
        std::string type;

        for (std::string sensorRequest : sensorRequestList)
        {
            logMessage("\tProcessing sensor request: %s\n", sensorRequest.c_str());
            if (sensorRequest.empty())
            {
                continue;
            }
            id = sensorRequest.substr(0, sensorRequest.find(':'));
            type = sensorRequest.substr(sensorRequest.find(':') + 1);

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
            logMessage("(!)Invalid metadata from request:%d!\n", request.c_str());
            return;
        }

        //Get sensor
        BaseSensor* sensor = getSensor(metadata.UID);
        if(sensor == nullptr)
        {
            logMessage("(!)Sensor with ID %s not found!\n", metadata.UID.c_str());
            return;
        }

        //Process request
        updateSensor(sensor, metadata.Data);
    }

    void print(std::string uid)
    {
        BaseSensor* sensor = getSensor(uid);
        printSensor(sensor);
    }

    void printAll()
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