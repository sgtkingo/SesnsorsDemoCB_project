/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
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
void manage(char* request)
{
    sensor_metadata metadata = parse_metadata(request);
}



