#ifndef __PARSER_H_
#define __PARSER_H_

/**********************
 *      TYPEDEFS
 **********************/
typedef struct  
{
  char* sensorType;
  char* sensorID;
  char* data;
}sensor_metadata;

/*********************
 *      DECLARES
 *********************/
int check_metadata(sensor_metadata* metadata);
sensor_metadata parse_metadata(char* request);

#endif /* EVENT_CB_H_ */