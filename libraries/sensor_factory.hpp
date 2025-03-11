#include "sensors.hpp"

#include <unordered_map>
#include <memory>
#include <string>
#include <functional> // For std::function


class SensorFactory {
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<BaseSensor>()>> registry;

public:
    // Register class by name
    template <typename T>
    void registerSensor(const std::string& name, const std::string& id) {
        registry[name] = []() -> std::unique_ptr<BaseSensor> {
            return std::make_unique<T>(id);
        };
    }

    // Create an object by name
    std::unique_ptr<BaseSensor> createSensor(const std::string& name) const {
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Sensor not found: " + name);
    }
};