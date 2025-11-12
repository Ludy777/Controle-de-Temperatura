#pragma once
#include "driver/gpio.h"
#include "esp_log.h"

class Ventoinha {
public:
    Ventoinha(gpio_num_t pino); //Construtor
    ~Ventoinha(); //Destrutor
    void inicio(); //Configuração do pino e do estado
    void liga(); //Liga a ventoinha
    void desliga(); //Desliga a ventoinha
    bool estaLigado(); //Indica o estado da ventoinha

private:
    gpio_num_t ventPino; //Pino em que está a ventoinha
    bool ventLigado; //Salva o ultimo estado configurado da ventoinha
};
