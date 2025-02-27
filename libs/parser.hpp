#ifndef __PARSER_H_
#define __PARSER_H_

/*********************
 *      INCLUDES
 *********************/
#include "exceptions.hpp"
#include "helpers.hpp"
#include <string>

/**********************
 *      TYPEDEFS
 **********************/
struct SensorMetadata
{
  int ID;
  std::string Type;
  std::string Data;
};

/*********************
 *      DECLARES
 *********************/


int CheckMetadata(SensorMetadata* metadata);


SensorMetadata ParseMetadata(std::string request);

#endif //__PARSER_H_