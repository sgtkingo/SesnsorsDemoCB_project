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
    return !metadata->UID.empty() && !metadata->Type.empty() && !metadata->Data.empty();
}

SensorMetadata ParseMetadata(std::string &request)
{
    SensorMetadata metadata;
    metadata.UID = "";
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
    std::string uid = getValueFromKeyValueLikeString(request, "id", '&');
    if(uid.empty())
    {
        throw ParseMetadataException("ParseMetadata", "ID not found in request!");
    }
    metadata.UID = uid;

    //Parse Type from request
    std::string type = getValueFromKeyValueLikeString(request, "type", '&');
    if(type.empty())
    {
        throw ParseMetadataException("ParseMetadata", "Type not found in request!");
    }
    metadata.Type = type;

    //Save the rest of the request as data
    metadata.Data = request;

    return metadata;
}



