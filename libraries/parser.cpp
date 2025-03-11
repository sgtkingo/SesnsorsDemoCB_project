/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

/*********************
 *      INCLUDES
 *********************/
#include "parser.hpp"

#include <cstdio>
#include <algorithm> // For std::transform

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
    if( metadata == nullptr )
    {
        return 0;
    }

    return !metadata->UID.empty() && !metadata->Data.empty();
}

int IsValid(const SensorMetadata* metadata, const std::string &uid)
{
    return CheckMetadata(metadata) && metadata->UID == uid;
}

SensorMetadata ParseMetadata(std::string &response)
{
    SensorMetadata metadata;
    metadata.UID = "";
    metadata.Status = "";
    metadata.Data = "";

    //Check request format
    if(response.size() < 1 || response[0] != '?')
    {
        return metadata;
    }

    //Get rid of the '?' character
    response.erase(0, 1);
    // Convert to lowercase
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);

    //Parse ID from request
    std::string uid = getValueFromKeyValueLikeString(response, "id", '&');
    if(!uid.empty())
    {
        metadata.UID = uid;
    }

    //Parse Status from request
    std::string status = getValueFromKeyValueLikeString(response, "status", '&');
    if(!status.empty())
    {
        metadata.Status = status;
    }
    
    //Save the rest of the request as data
    metadata.Data = response;

    return metadata;
}



