#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "Ventoinha.h"
#include "Sensor.h"
#include "Dimmer.h"
#include "ZeroCross.h"

//Definição dos pinos que serão utilizados
const gpio_num_t TEMP_PIN = GPIO_NUM_4;
const gpio_num_t VENT_PIN  = GPIO_NUM_23;
const gpio_num_t TRIAC_PIN = GPIO_NUM_26;
const gpio_num_t ZC_PIN = GPIO_NUM_27;

//Definições das temperatura de comparação
float tempAlvo = 26.0;

//Precisa ser criado fora da main por causa da interrupção
Dimmer dimmer(TRIAC_PIN);

// Interrupção de zero-cross chama o dimmer
void onZeroCrossISR() {
    dimmer.onZeroCross();
}

//Precisa de extern C porque o esp32 só trabalha com C puro e assim não dá problema
extern "C" void app_main(void) {
    
    //Criação de objeto das classes
    Sensor sensor(TEMP_PIN);
    Ventoinha ventoinha(VENT_PIN);
    ZeroCross zc(ZC_PIN);

    //Inicia as classes corretamente
    sensor.inicio();
    ventoinha.inicio();
    dimmer.inicio();
    zc.inicio(onZeroCrossISR);

    //Variáveis para controle do pwm
    int ultimoPwm = 0, pwm = 0;

    //Mesma coisa que cout, só que para o esp32
    ESP_LOGI("", "Controle de temperatura iniciado.");

    while (true) {
        //Leitura da temperatura do sensor
        float temperatura = sensor.lerCelsius();        

        //Comparações para saber a potência do pwm
        if (temperatura > tempAlvo + 1.0){
            pwm = 5;
        }
        else if (temperatura > tempAlvo){
            pwm = 10;
        }
        else if (temperatura > tempAlvo - 1.0){
            pwm = 15;
        }
        else{
            pwm = 100;
        }

        //Só muda o pwm se for diferente do que já está ativo
        //para não forçar 
        if(pwm != ultimoPwm){
            ultimoPwm = pwm;
            dimmer.setPwm(pwm);
        }

        //Liga ou desliga a ventoinha com base na temperatura do sensor lida
        if (temperatura > tempAlvo && !ventoinha.estaLigado()) {
            ventoinha.liga();
        } else if (temperatura < (tempAlvo-1.0) && ventoinha.estaLigado()) {
            ventoinha.desliga();
        }        

        //Delay do freeRtos
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}