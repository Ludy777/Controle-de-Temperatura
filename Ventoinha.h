#pragma once
#include "driver/gpio.h"
#include "esp_log.h"

class Ventoinha {
private:
    gpio_num_t ventPino;
    bool ventLigado;
public:
    Ventoinha(gpio_num_t pino);
    void inicio();
    void liga();
    void desliga();
    bool estaLigado();
};
