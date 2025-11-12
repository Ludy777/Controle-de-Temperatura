#include "Ventoinha.h"

//Construtor da classe ventoinha
Ventoinha::Ventoinha(gpio_num_t pino) {
    ventPino = pino;
    ventLigado = false;
}

//Inicia a classe
void Ventoinha::inicio() {
    gpio_set_direction(ventPino, GPIO_MODE_OUTPUT);
    gpio_set_level(ventPino, 0);
    ESP_LOGI("", "Ventoinha configurada no pino %d", ventPino);
}

//Liga a ventoinha caso não esteja ligada
void Ventoinha::liga() {
    if (!ventLigado) {
        gpio_set_level(ventPino, 1);
        ventLigado = true;
        ESP_LOGI("", "Ventoinha LIGADA");
    }
}

//Desliga a ventoinha caso esteja ligada
void Ventoinha::desliga() {
    if (ventLigado) {
        gpio_set_level(ventPino, 0);
        ventLigado = false;
        ESP_LOGI("", "Ventoinha DESLIGADA");
    }
}

//Retorna o estado atual da ventoinha
bool Ventoinha::estaLigado() {
    return ventLigado;
}

//Destrutor da classe
Ventoinha::~Ventoinha(){
    ventLigado = false;
    gpio_set_level(ventPino, 0);
}