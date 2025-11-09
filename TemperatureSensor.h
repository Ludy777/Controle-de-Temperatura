#pragma once
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TemperatureSensor {
private:
    gpio_num_t pin;
    float lastTemperature;
    float readRawTemperature();

public:
    TemperatureSensor(gpio_num_t dataPin);
    void begin();
    float readCelsius();
};
