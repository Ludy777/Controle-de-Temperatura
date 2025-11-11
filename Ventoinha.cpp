#include "Ventoinha.h"

static const char *TAG = "Ventoinha";

Ventoinha::Ventoinha(gpio_num_t pino) {
    ventPino = pino;
    ventLigado = false;
}

Ventoinha::~Ventoinha(){
    ventLigado = false;
    gpio_set_level(ventPino, 0);
}

void Ventoinha::inicio() {
    gpio_set_direction(ventPino, GPIO_MODE_OUTPUT);
    gpio_set_level(ventPino, 0);
    ESP_LOGI(TAG, "Ventoinha configurada no pino %d", ventPino);
}

void Ventoinha::liga() {
    if (!ventLigado) {
        gpio_set_level(ventPino, 1);
        ventLigado = true;
        ESP_LOGI(TAG, "⚙️ Ventoinha LIGADA");
    }
}

void Ventoinha::desliga() {
    if (ventLigado) {
        gpio_set_level(ventPino, 0);
        ventLigado = false;
        ESP_LOGI(TAG, "💤 Ventoinha DESLIGADA");
    }
}

bool Ventoinha::estaLigado() {
    return ventLigado;
}
