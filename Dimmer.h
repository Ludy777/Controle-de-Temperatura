#pragma once
#include "driver/gpio.h"
#include "esp_timer.h"

class Dimmer {
private:
    gpio_num_t triacPin;
    int powerLevel;
    int delayMicroseconds;

public:
    explicit Dimmer(gpio_num_t pin);
    ~Dimmer();
    void inicio();
    void setPwm(int percent);
    void onZeroCross();
    int getPower() const { return powerLevel; }
};
