#pragma once
#include "driver/gpio.h"
#include "esp_log.h"

class Ventoinha {

private:
    gpio_num_t ventPino; //Pino em que está a ventoinha
    bool ventLigado; //Salva o ultimo estado configurado da ventoinha


public:
    //Construtor da classe ventoinha
    Ventoinha(gpio_num_t pino){
        ventPino = pino;
        ventLigado = false;
    }

    //Destrutor da classe
    ~Ventoinha(){
        ventLigado = false;
        gpio_set_level(ventPino, 0);
    }

    //Configuração do pino e do estado
    void inicio(){
        gpio_set_direction(ventPino, GPIO_MODE_OUTPUT);
        gpio_set_level(ventPino, 0);
        ESP_LOGI("", "Ventoinha configurada no pino %d", ventPino);
    }

    //Liga a ventoinha caso não esteja ligada
    void liga(){
        if (!ventLigado) {
            gpio_set_level(ventPino, 1);
            ventLigado = true;
            ESP_LOGI("", "Ventoinha LIGADA");
        }
    } 

    //Desliga a ventoinha caso esteja ligada
    void desliga(){
        if (ventLigado) {
            gpio_set_level(ventPino, 0);
            ventLigado = false;
            ESP_LOGI("", "Ventoinha DESLIGADA");
        }
    }

    //Retorna o estado atual da ventoinha
    bool estaLigado(){
        return ventLigado;
    }
};
