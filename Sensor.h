#pragma once
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Sensor {
private:
    gpio_num_t pino; //Variavel indicando o pino que vai ser usado
    float ultimaTemp; //Ultima media válida de temperatura
    float leituraTemp(); //Faz a leitura da temperatura

    //Funções para a leitura de temperatura do sensor
    static void sensorTemp_reset(gpio_num_t pin);
    static void sensorTemp_write_bit(gpio_num_t pin, int bit);
    static int sensorTemp_read_bit(gpio_num_t pin);
    static void sensorTemp_write_byte(gpio_num_t pin, uint8_t byte);
    static uint8_t sensorTemp_read_byte(gpio_num_t pin) ;

public:
    Sensor(gpio_num_t dataPin); //Construtor
    ~Sensor(); //Destrutor
    void inicio(); //Configuração inicial
    float lerCelsius(); //Retorna a temperatura lida para quem chamou
};
