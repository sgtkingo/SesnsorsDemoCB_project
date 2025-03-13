/*
* Copyright 2024 MTA
* Author: Ing. Jiri Konecny
*/

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

/**
 * @brief Structure to hold sensor metadata.
 */
struct SensorMetadata
{
  std::string UID;
  std::string Status;
  std::string Data;
};

/*********************
 *      DECLARES
 *********************/

 /**
 * @brief Check if the metadata is valid.
 * 
 * @param metadata The metadata to check.
 * @return true if the metadata is valid, false otherwise.
 */
int IsValid(const SensorMetadata* metadata, const std::string &uid);

/**
 * @brief Check if the metadata is valid and matches the given UID.
 * 
 * @param metadata The metadata to check.
 * @param uid The UID to match.
 * @return true if the metadata is valid and matches the UID, false otherwise.
 */
int CheckMetadata(const SensorMetadata* metadata);

/**
 * @brief Parse metadata from a request string.
 * 
 * @param response The request string to parse.
 * @param caseSensitive Flag to indicate if parsing should be case-sensitive.
 * @return The parsed metadata.
 */
SensorMetadata ParseMetadata(std::string &request, bool caseSensitive = false);

#endif //__PARSER_H_