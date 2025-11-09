#include "ZeroCross.h"
#include "esp_log.h"

void (*ZeroCross::zcCallback)() = nullptr;

ZeroCross::ZeroCross(gpio_num_t pin) : zcPin(pin) {}

void ZeroCross::begin(void (*callback)()) {
    zcCallback = callback;

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << zcPin);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(zcPin, isrHandler, nullptr);

    ESP_LOGI("ZeroCross", "Configuração concluída no pino %d", zcPin);
}

void IRAM_ATTR ZeroCross::isrHandler(void *arg) {
    if (zcCallback) zcCallback();
}
