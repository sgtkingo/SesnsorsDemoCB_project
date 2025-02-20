#include "libs/sensors.hpp"

int main() {
    std::string configData = "?id=0&Resolution=10";
    std::string updateData = "?id=0&Value=255";
    ADC* adc = createADC(0);

    printSensor(adc);
    configSensor(adc, configData);
    configSensor(adc, "blbost");
    updateSensor(adc, updateData);
    updateSensor(adc, "???");
    printSensor(adc);

    return 0;
}
