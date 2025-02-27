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
int CheckMetadata(const SensorMetadata* metadata)
{
    return metadata->ID != -1 && !metadata->Type.empty() && !metadata->Data.empty();
}

SensorMetadata ParseMetadata(std::string &request)
{
    SensorMetadata metadata;
    metadata.ID = -1;
    metadata.Type = "";
    metadata.Data = "";

    //Check request format
    if(request.size() < 1 || request[0] != '?')
    {
        throw ParseMetadataException("ParseMetadata", "Invalid request format!");
    }
    else
    {
        //Get rid of the '?' character
        request.erase(0, 1);
    }

    //Parse ID from request
    try
    {
        std::string id = getValueFromKeyValueLikeString(request, "id", '&');
        if(id.empty())
        {
            throw ParseMetadataException("ParseMetadata", "ID not found in request!");
        }
        metadata.ID = convertStringToType<int>(id);
    }
    catch(const Exception& e)
    {
        throw ParseMetadataException("ParseMetadata", new Exception(e));
    }
    
    metadata.Type = getValueFromKeyValueLikeString(request, "type", '&');
    metadata.Data = request;

    return metadata;
}



