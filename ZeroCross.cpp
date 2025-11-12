#include "ZeroCross.h"
#include "esp_log.h"

//Ponteiro para a interrupção
void (*ZeroCross::chamada)() = nullptr;

//Construtor da classe zeroCross
ZeroCross::ZeroCross(gpio_num_t pin) : zcPino(pin) {}

//Inicia a classe configurando o pino e a interrupção no pino
void ZeroCross::inicio(void (*callback)()) {
    chamada = callback;

    gpio_config_t io_conf = {}; //Array de configurações
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << zcPino);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(zcPino, isrHandler, nullptr);

    ESP_LOGI("", "Configuração do ZeroCross concluída no pino %d", zcPino);
}

//Chama a interrupção
void IRAM_ATTR ZeroCross::isrHandler(void *arg) {
    if (chamada) chamada();
}

//Destrutor da classe
ZeroCross::~ZeroCross() {
    gpio_isr_handler_remove(zcPino);
}