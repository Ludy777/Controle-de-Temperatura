#include "FanController.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

FanController::FanController(gpio_num_t fanPin, TemperatureSensor* tempSensor,
                             float tempOn, float tempOff)
    : fanPin(fanPin), tempOn(tempOn), tempOff(tempOff), fanOn(false), sensor(tempSensor) {}

void FanController::begin() {
    gpio_reset_pin(fanPin);
    gpio_set_direction(fanPin, GPIO_MODE_OUTPUT);
    gpio_set_level(fanPin, 0);

    sensor->init();
    ESP_LOGI(TAG, "Controle de ventoinha inicializado no pino %d", fanPin);
}

void FanController::update() {
    float temp = sensor->readTemperature();

    if (temp < -100.0f) { // erro de leitura
        ESP_LOGW(TAG, "Ignorando leitura inválida");
        return;
    }

    ESP_LOGW(TAG, "(%.2f °C)", temp);

    if (!fanOn && temp > tempOn) {
        gpio_set_level(fanPin, 1);
        fanOn = true;
        ESP_LOGW(TAG, "⚙️ Ventoinha LIGADA (%.2f °C)", temp);
    } 
    else if (fanOn && temp < tempOff) {
        gpio_set_level(fanPin, 0);
        fanOn = false;
        ESP_LOGW(TAG, "💤 Ventoinha DESLIGADA (%.2f °C)", temp);
    }
}
