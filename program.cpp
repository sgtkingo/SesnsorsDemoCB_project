#include "libs/sensors.hpp"

int main() {
    std::string configData = "?id=0&Resolution=10";
    std::string updateData = "?id=0&Value=255";

    //You can use generic createSensor function to create sensors
    ADC* adc = createSensor<ADC>(0);
    printSensor(adc);
    configSensor(adc, configData);
    configSensor(adc, "blbost");
    updateSensor(adc, updateData);
    updateSensor(adc, "???");
    printSensor(adc);

    TH* th = createSensor<TH>(1);
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

    //Or use the getValue function
    double temp = th->getValue<double>("Temperature");
    int hum = th->getValue<int>("Humidity");

    printf("Temperature: %f [%s]\n", temp, th->getUnits("Temperature").c_str());
    printf("Humidity: %d [%s]\n", hum, th->getUnits("Humidity").c_str());

    //Works on configurations too
    int precision = th->getConfig<int>("Precision");
    //Get units is universal function for Configs and Values
    std::string precisionUnit = th->getUnits("Precision");
    printf("Precision: %d [%s]\n", precision, precisionUnit.c_str());

    //Ovewrite old sensor with new one
    adc = createSensor<ADC>(2);
    adc->print();

    //Dont forget to delete sensors
    delete adc;
    delete th;

    return 0;
}
