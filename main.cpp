#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "FanController.h"
#include "TemperatureSensor.h"

extern "C" void app_main(void) {
    const gpio_num_t TEMP_PIN = GPIO_NUM_4;
    const gpio_num_t FAN_PIN  = GPIO_NUM_23;

    TemperatureSensor sensor(TEMP_PIN);
    FanController fan(FAN_PIN);

    sensor.begin();
    fan.begin();

    const float TEMP_ON  = 26.0f;
    const float TEMP_OFF = 25.0f;

    ESP_LOGI("", "Controle de temperatura iniciado.");

    while (true) {
        float temp = sensor.readCelsius();

        if (temp > TEMP_ON && !fan.isOn()) {
            fan.turnOn();
        } else if (temp < TEMP_OFF && fan.isOn()) {
            fan.turnOff();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
