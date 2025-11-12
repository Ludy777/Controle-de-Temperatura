#pragma once
#include "driver/gpio.h"
#include "esp_attr.h"

class ZeroCross {
public:
    ZeroCross(gpio_num_t pin); //Construtor da classe
    ~ZeroCross(); //Destrutor da classe
    void inicio(void (*callback)()); //Configuração do pino e interrupção nele
    static void IRAM_ATTR isrHandler(void *arg);  //Interrupção do ZeroCross

private:
    gpio_num_t zcPino; //Pino a ser configurado
    static void (*chamada)();//Variavel estatica para a função interrupção
};
