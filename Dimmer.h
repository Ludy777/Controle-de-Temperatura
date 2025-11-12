#pragma once
#include "driver/gpio.h"
#include "esp_timer.h"

class Dimmer {
private:
    gpio_num_t triacPin; //Variavel indicando o pino que vai ser usado
    int potPwm; //Potência do pwm
    int delayMicrosegundos; //Segundos de delay que vai

public:
    Dimmer(gpio_num_t pin); //Construtor
    ~Dimmer(); //Destrutor
    void inicio(); //Configuração inicial
    void setPwm(int pwm); //Ajusta o delay do pwm
    void onZeroCross(); //Aplica o delay do pwm no pino
};
