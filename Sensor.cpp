#include "Sensor.h"
#include "esp_rom_sys.h" // Para esp_rom_delay_us()

//Construtor da classe sensor
Sensor::Sensor(gpio_num_t dataPin) {
    pino = dataPin;
    ultimaTemp = 25.0; // valor inicial
}

//Inicia a classe
void Sensor::inicio() {
    gpio_set_direction(pino, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pino, GPIO_PULLUP_ONLY);
    ESP_LOGI("", "Sensor DS18B20 inicializado no pino %d", pino);
}

// Faz a laitura do sensor
float Sensor::leituraTemp() {
    sensorTemp_reset(pino); //Sincroniza o sinal para iniciar a comunicação
    sensorTemp_write_byte(pino, 0xCC); // Indica que vai conversar com o sensor
    sensorTemp_write_byte(pino, 0x44); // Inicia a leitura da temperatura

    vTaskDelay(pdMS_TO_TICKS(750)); // tempo de conversão do DS18B20

    sensorTemp_reset(pino);
    sensorTemp_write_byte(pino, 0xCC); // Indica que vai conversar com o sensor
    sensorTemp_write_byte(pino, 0xBE); // Indica que vai ler a temperatura

    uint8_t tempLSB = sensorTemp_read_byte(pino); // Parte menos significativa da leitura
    uint8_t tempMSB = sensorTemp_read_byte(pino); // Parte mais significativa da leitura

    int16_t tempBruta = (tempMSB << 8) | tempLSB; // Monta a temperatura
    return tempBruta / 16.0f; // Converte o dado de temperatura para ºC
}

// Pega a temperatura lida e faz uma média para ajudar no tratamento posterior
float Sensor::lerCelsius() {
    float newTemp = leituraTemp();

    // filtro de suavização exponencial (média móvel leve)
    ultimaTemp = (0.8f * ultimaTemp) + (0.2f * newTemp);

    ESP_LOGI("","Temp lida: %.2f °C", ultimaTemp);
    return ultimaTemp;
}

// Destrutor da classe
Sensor::~Sensor(){
    ultimaTemp = 0;
}

/*
    Funções necessárias para funcionamento do sensor DS18B20
    O sensor só faz a leitura e envia para o controlador quando é pedido
*/
void Sensor::sensorTemp_reset(gpio_num_t pin) {
    gpio_set_level(pin, 0);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    esp_rom_delay_us(480);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    esp_rom_delay_us(480);
}

void Sensor::sensorTemp_write_bit(gpio_num_t pin, int bit) {
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, 0);
    if (bit) {
        esp_rom_delay_us(10);
        gpio_set_level(pin, 1);
        esp_rom_delay_us(55);
    } else {
        esp_rom_delay_us(65);
        gpio_set_level(pin, 1);
        esp_rom_delay_us(5);
    }
}

int Sensor::sensorTemp_read_bit(gpio_num_t pin) {
    int bit;
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, 0);
    esp_rom_delay_us(3);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    esp_rom_delay_us(10);
    bit = gpio_get_level(pin);
    esp_rom_delay_us(53);
    return bit;
}

void Sensor::sensorTemp_write_byte(gpio_num_t pin, uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        sensorTemp_write_bit(pin, byte & 0x01);
        byte >>= 1;
    }
}

uint8_t Sensor::sensorTemp_read_byte(gpio_num_t pin) {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        int bit = sensorTemp_read_bit(pin);
        value |= (bit << i);
    }
    return value;
}