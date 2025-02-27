/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

/*********************
 *      INCLUDES
 *********************/
#include "manager.hpp"


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *     VARIABLES
 **********************/

/*********************
 *      DEFINES
 *********************/
void ManageRequest(std::string request)
{
    SensorMetadata metadata = ParseMetadata(request);
    if(CheckMetadata(&metadata))
    {
        //TODO: Find sensor by ID and call update
    }
}



