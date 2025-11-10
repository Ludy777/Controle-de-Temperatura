#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "Ventoinha.h"
#include "Sensor.h"
#include "Dimmer.h"
#include "ZeroCross.h"

const gpio_num_t TEMP_PIN = GPIO_NUM_4;
const gpio_num_t FAN_PIN  = GPIO_NUM_23;
const gpio_num_t TRIAC_PIN = GPIO_NUM_26;
const gpio_num_t ZC_PIN = GPIO_NUM_27;

const float TEMP_ON  = 26.0f;
const float TEMP_OFF = 25.0f;
float setpoint = 27.0f;

//Precisa ser criado fora da main por causa da interrupção
Dimmer dimmer(TRIAC_PIN);

// ISR de zero-cross chama o dimmer
void onZeroCrossISR();

extern "C" void app_main(void) {
    
    Sensor sensor(TEMP_PIN);
    Ventoinha fan(FAN_PIN);
    ZeroCross zc(ZC_PIN);

    sensor.inicio();
    fan.inicio();
    dimmer.begin();
    zc.begin(onZeroCrossISR);

    int lastPower = 0, power = 0;

    ESP_LOGI("", "Controle de temperatura iniciado.");

    while (true) {
        float temp = sensor.lerCelsius();        

        if (temp > setpoint + 1.0f){
            power = 5;
        }
        else if (temp > setpoint){
            power = 10;
        }
        else if (temp > setpoint - 1.0f){
            power = 15;
        }
        else{
            power = 100;
        }

        if(power != lastPower){
            lastPower = power;
            dimmer.setPower(power);
        }

        if (temp > TEMP_ON && !fan.estaLigado()) {
            fan.liga();
        } else if (temp < TEMP_OFF && fan.estaLigado()) {
            fan.desliga();
        }        

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void onZeroCrossISR() {
    dimmer.onZeroCross();
}