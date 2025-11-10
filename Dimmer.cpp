#include "Dimmer.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

static const char *TAG = "Dimmer";

Dimmer::Dimmer(gpio_num_t pin) {
    triacPin = pin;
    powerLevel = 0;
    delayMicroseconds = 8000;
    setPwm(0);
}

void Dimmer::inicio() {
    gpio_set_direction(triacPin, GPIO_MODE_OUTPUT);
    gpio_set_level(triacPin, 0);
    ESP_LOGI(TAG, "Dimmer iniciado no pino %d", triacPin);
}

void Dimmer::setPwm(int pwm) {
    if (pwm < 0) pwm = 0;
    if (pwm > 100) pwm = 100;
    powerLevel = pwm;
    delayMicroseconds = 8000 - (pwm * 80);

    ESP_LOGI("","PWM lampada: %d", pwm);
}

void Dimmer::onZeroCross() {
    esp_rom_delay_us(delayMicroseconds);
    gpio_set_level(triacPin, 1);
    esp_rom_delay_us(100);
    gpio_set_level(triacPin, 0);
}
