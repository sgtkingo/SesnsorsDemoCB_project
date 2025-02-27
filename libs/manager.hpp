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

/**********************
 *      TYPEDEFS
 **********************/



/*********************
 *      DECLARES
 *********************/


void ManageRequest(std::string request);

#endif //__MANAGER_H_