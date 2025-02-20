/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include "parser.hpp"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *     VARIABLES
 **********************/

/*********************
 *      DEFINES
 *********************/
int check_metadata(sensor_metadata* metadata)
{
    return metadata->sensorType != NULL && metadata->sensorID != NULL && metadata->data != NULL;
}

sensor_metadata parse_metadata(char* request)
{
    sensor_metadata metadata;

    metadata.sensorType = strtok(request, "&");
    metadata.sensorID = strtok(NULL, "&");
    metadata.data = strtok(NULL, "&");

    return metadata;
}


