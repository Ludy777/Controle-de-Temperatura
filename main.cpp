#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "Ventoinha.h"
#include "Sensor.h"
#include "Dimmer.h"
#include "ZeroCross.h"

const gpio_num_t TEMP_PIN = GPIO_NUM_4;
const gpio_num_t VENT_PIN  = GPIO_NUM_23;
const gpio_num_t TRIAC_PIN = GPIO_NUM_26;
const gpio_num_t ZC_PIN = GPIO_NUM_27;

const float TEMP_ON  = 26.0f;
const float TEMP_OFF = 25.0f;
float tempAlvo = 26.0f;

//Precisa ser criado fora da main por causa da interrupção
static Dimmer dimmer(TRIAC_PIN);

// ISR de zero-cross chama o dimmer
void onZeroCrossISR();

extern "C" void app_main(void) {
    
    Sensor sensor(TEMP_PIN);
    Ventoinha ventoinha(VENT_PIN);
    ZeroCross zc(ZC_PIN);

    sensor.inicio();
    ventoinha.inicio();
    dimmer.inicio();
    zc.inicio(onZeroCrossISR);

    int ultimoPwm = 0, pwm = 0;

    ESP_LOGI("", "Controle de temperatura iniciado.");

    while (true) {
        float temperatura = sensor.lerCelsius();        

        if (temperatura > tempAlvo + 1.0f){
            pwm = 5;
        }
        else if (temperatura > tempAlvo){
            pwm = 10;
        }
        else if (temperatura > tempAlvo - 1.0f){
            pwm = 15;
        }
        else{
            pwm = 100;
        }

        if(pwm != ultimoPwm){
            ultimoPwm = pwm;
            dimmer.setPwm(pwm);
        }

        if (temperatura > TEMP_ON && !ventoinha.estaLigado()) {
            ventoinha.liga();
        } else if (temperatura < TEMP_OFF && ventoinha.estaLigado()) {
            ventoinha.desliga();
        }        

        //Delay do freeRtos
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void onZeroCrossISR() {
    dimmer.onZeroCross();
}