#pragma once
#include "driver/gpio.h"
#include "esp_log.h"

class FanController {
private:
    gpio_num_t fanPin;
    bool fanOn;
public:
    FanController(gpio_num_t pin);
    void begin();
    void turnOn();
    void turnOff();
    bool isOn();
};
