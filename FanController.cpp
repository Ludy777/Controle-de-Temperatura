#include "FanController.h"

static const char *TAG = "FanController";

FanController::FanController(gpio_num_t pin) {
    fanPin = pin;
    fanOn = false;
}

void FanController::begin() {
    gpio_set_direction(fanPin, GPIO_MODE_OUTPUT);
    gpio_set_level(fanPin, 0);
    ESP_LOGI(TAG, "Ventoinha configurada no pino %d", fanPin);
}

void FanController::turnOn() {
    if (!fanOn) {
        gpio_set_level(fanPin, 1);
        fanOn = true;
        ESP_LOGI(TAG, "⚙️ Ventoinha LIGADA");
    }
}

void FanController::turnOff() {
    if (fanOn) {
        gpio_set_level(fanPin, 0);
        fanOn = false;
        ESP_LOGI(TAG, "💤 Ventoinha DESLIGADA");
    }
}

bool FanController::isOn() {
    return fanOn;
}
