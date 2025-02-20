#include "libs/sensors.hpp"

int main() {
    std::string configData = "?id=0&Resolution=10";
    std::string updateData = "?id=0&Value=255";

    ADC* adc = ADC::create(0);
    printSensor(adc);
    configSensor(adc, configData);
    configSensor(adc, "blbost");
    updateSensor(adc, updateData);
    updateSensor(adc, "???");
    printSensor(adc);

    TH* th = TH::create(1);
    printSensor(th);
    configSensor(th, "Precision=3");
    updateSensor(th, "Temperature=25.5&Humidity=50");
    printSensor(th);

    //How to get certain value from sensor
    std::string tempValue = th->Values["Temperature"].Value;
    std::string tempUnit = th->Values["Temperature"].Unit;
    DataType tempDt = th->Values["Temperature"].DataType;

    std::string huminidyValue = th->Values["Humidity"].Value;
    std::string huminidyUnit = th->Values["Humidity"].Unit;
    DataType huminidyDt = th->Values["Humidity"].DataType;

    //How to convert string values to numerical values
    double doubleTempValue = std::stod(tempValue);
    int intHuminidyValue = std::stoi(huminidyValue);

    printf("Temperature: %f [%s]\n", doubleTempValue, tempUnit.c_str());
    printf("Humidity: %d [%s]\n", intHuminidyValue, huminidyUnit.c_str());

    return 0;
}
