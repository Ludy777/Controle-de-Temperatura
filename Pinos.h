#pragma once
#include "driver/gpio.h"
#include "esp_timer.h"

class Pinos{
    private:
        gpio_num_t pino;

    public:
        Pinos();
        ~Pinos();
        void configPino();
};