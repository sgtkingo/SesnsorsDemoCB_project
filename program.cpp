#include <stdio.h>
#include <string>
#include <cstdio>           ///< For sscanf
#include <unordered_map>

//Exception testing
//#include "libs/exceptions.hpp"

//Sensors testing
//#include "libs/sensors.hpp"

int main() {
    printf("Program READY!\n");

    int scanStatus = 0;
    int resolution = 0;
    float temp = 0;

    std::unordered_map<std::string, std::string> parameters;
    parameters["Value"] = "%f";
    parameters["Resolution"] = "%d";
    std::string format = "";
    for(auto& parameter : parameters) {
        printf("Parameter: %s\n", parameter.first.c_str());
        printf("Value: %s\n", parameter.second.c_str());
        format += parameter.first + "=" + parameter.second + "&";
    }
    printf("Format: %s\n", format.c_str());

    scanStatus = sscanf("Resolution=10&Temp=255.0", format.c_str(), &resolution, &temp);
    //scanStatus = sscanf("Resolution=10&Temp=255.0", "Resolution=%d", &resolution);
    //scanStatus = sscanf("Resolution=10&Temp=255.0", "&Temp=%f", &temp);
    printf("Resolution: %d\n", resolution);
    printf("Temp: %f\n", temp);
    /*
    std::unordered_map<std::string, std::string> parameters;
    parameters["Resolution"] = "10";
    parameters["Value"] = "3.14";

    for(auto& parameter : parameters) {
        printf("Parameter: %s\n", parameter.first.c_str());
        printf("Value: %s\n", parameter.second.c_str());

        scanStatus = sscanf(parameter.first.c_str(), "Resolution=%d", &resolution);
    }
    */


    //Sensors testing
    /*
    ADC* adc = createADC(1);
    printSensor(adc);

    updateSensor(adc, "Value=3.14");
    configSensor(adc, "Resolution=0");
    //configSensor(adc, "Resolution=10");

    printSensor(adc);
    */
    return 0;
}
