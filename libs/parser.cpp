/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

/*********************
 *      INCLUDES
 *********************/
#include "parser.hpp"
#include <cstdio>

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *     VARIABLES
 **********************/

/*********************
 *      DEFINES
 *********************/
int CheckMetadata(SensorMetadata* metadata)
{
    return metadata->ID != -1 && !metadata->Type.empty() && !metadata->Data.empty();
}

SensorMetadata ParseMetadata(std::string request)
{
    SensorMetadata metadata;
    
    /*
    metadata.Type = strtok(request, "&");
    metadata.ID = strtok(NULL, "&");
    metadata.Data = strtok(NULL, "&");
    */

    return metadata;
}


