#pragma once
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Sensor {
private:
    gpio_num_t pino;
    float ultimaTemp;
    float leituraTemp();

public:
    Sensor(gpio_num_t dataPin);
    void inicio();
    float lerCelsius();
};
