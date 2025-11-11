#pragma once
#include "driver/gpio.h"
#include "esp_attr.h"  // <-- importante: define IRAM_ATTR

class ZeroCross {
public:
    explicit ZeroCross(gpio_num_t pin);
    ~ZeroCross();
    void inicio(void (*callback)());
    static void IRAM_ATTR isrHandler(void *arg);  // posição correta
private:
    gpio_num_t zcPin;
    static void (*zcCallback)();
};
