#pragma once
#include "driver/gpio.h"
#include "esp_log.h"
#include "TemperatureSensor.h"

class FanController {
private:
    static constexpr const char* TAG = "FanController";
    gpio_num_t fanPin;
    float tempOn;
    float tempOff;
    bool fanOn;
    TemperatureSensor* sensor;

public:
    FanController(gpio_num_t fanPin, TemperatureSensor* tempSensor,
                  float tempOn = 26.2f, float tempOff = 25.8f);

    void begin();
    void update();
};
