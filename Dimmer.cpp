#include "Dimmer.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

static const char *TAG = "Dimmer";

Dimmer::Dimmer(gpio_num_t pin) {
    triacPin = pin;
    powerLevel = 0;
    delayMicroseconds = 8000;
}

void Dimmer::begin() {
    gpio_set_direction(triacPin, GPIO_MODE_OUTPUT);
    gpio_set_level(triacPin, 0);
    ESP_LOGI(TAG, "Dimmer iniciado no pino %d", triacPin);
}

void Dimmer::setPower(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    powerLevel = percent;
    delayMicroseconds = 8000 - (percent * 80);
}

void Dimmer::onZeroCross() {
    esp_rom_delay_us(delayMicroseconds);
    gpio_set_level(triacPin, 1);
    esp_rom_delay_us(100);
    gpio_set_level(triacPin, 0);
}
