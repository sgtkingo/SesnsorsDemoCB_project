#include "libraries/manager.hpp"

SensorManager Manager = SensorManager();

int main() {
    std::string configData = "?id=0&type=ADC&Resolution=10";
    std::string configData1 = "?type=ADC&Resolution=10";
    std::string configData2 = "?id=X&type=ADC&Resolution=10";
    std::string updateData = "?id=0&Value=255";
    std::string updateData1 = "?id=0&type=ADC&Value=255";

    std::string initRequest = "?0:ADC&1:ADC&2:TH";

    //Manager.init();
    Manager.init(true);
    Manager.print();

    Manager.reconstruct();
    Manager.resync();
    Manager.redraw();

    Manager.print();
    Manager.erase();

    /*
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

    //How to set value to sensor
    th->setValue("Temperature", "30.2");
    th->setValue("Humidity", "45");
    //How to set config to sensor
    th->setConfig("Precision", "2");
    printSensor(th);

    //How to synchronize sensor
    th->synchronize();

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

    printf("Temperature: %f [%s]\n", temp, th->getValueUnits("Temperature").c_str());
    printf("Humidity: %d [%s]\n", hum, th->getValueUnits("Humidity").c_str());

    //Works on configurations too
    int precision = th->getConfig<int>("Precision");
    //Get units is universal function for Configs and Values
    std::string precisionUnit = th->getConfigUnits("Precision");
    printf("Precision: %d [%s]\n", precision, precisionUnit.c_str());

    //Ovewrite old sensor with new one
    adc = createSensor<ADC>(2);
    adc->print();
    */

    //Dont forget to delete sensors
    //delete adc;
    //delete th;

    return 0;
}
