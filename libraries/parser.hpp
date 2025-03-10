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
  std::string UID;
  std::string Type;
  std::string Data;
};

/*********************
 *      DECLARES
 *********************/


int CheckMetadata(const SensorMetadata* metadata);


SensorMetadata ParseMetadata(std::string &request);

#endif //__PARSER_H_