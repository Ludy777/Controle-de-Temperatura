#include "Sensor.h"
#include "esp_rom_sys.h" // Para esp_rom_delay_us()

//Construtor da classe sensor
Sensor::Sensor(gpio_num_t dataPin) {
    pino = dataPin;
    ultimaTemp = 25.0; // valor inicial
}

//Inicia a classe
void Sensor::inicio() {
    gpio_set_direction(pino, GPIO_MODE_INPUT_OUTPUT_OD); //Configura o pino para ser tanto entrada como saída
    gpio_set_pull_mode(pino, GPIO_PULLUP_ONLY);//Configura o pullup interno do esp32 no pino
    ESP_LOGI("", "Sensor DS18B20 inicializado no pino %d", pino);
}

// Faz a leitura do sensor
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
    return tempBruta / 16.0; // Converte o dado de temperatura para ºC
}

// Pega a temperatura lida e faz uma média para ajudar no tratamento posterior
float Sensor::lerCelsius() {
    float newTemp = leituraTemp();

    // Faz uma média com o valor atual e o valor lido
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
    gpio_set_level(pin, 0); //Ajusta no nivel lógico do pino em 0, baixo
    gpio_set_direction(pin, GPIO_MODE_OUTPUT); //Ajusta o pino como saida
    esp_rom_delay_us(480); //Delay
    gpio_set_direction(pin, GPIO_MODE_INPUT); //Ajusta o pino como entrada
    esp_rom_delay_us(480); //Delay
}

//Escreve um bit para o sensor, esse bit vai ser nivel lógico alto ou baixo no pino
void Sensor::sensorTemp_write_bit(gpio_num_t pin, int bit) {
    gpio_set_direction(pin, GPIO_MODE_OUTPUT); //Ajusta o pino como saida
    gpio_set_level(pin, 0); //Ajusta no nivel lógico do pino em 0, baixo
    if (bit) {//Indica pro sensor que o nivel lógico do bit transmitido é nivel alto
        esp_rom_delay_us(10);//Delay
        gpio_set_level(pin, 1);//Ajusta no nivel lógico do pino em 1, alto
        esp_rom_delay_us(55);//Delay
    } else {//Indica pro sensor que o nivel lógico do bit transmitido é nivel baixo
        esp_rom_delay_us(65);//Delay
        gpio_set_level(pin, 1);//Ajusta no nivel lógico do pino em 1, alto
        esp_rom_delay_us(5);//Delay
    }
}

//Lê um bit enviado pelo sensor, esse bit vai ser nivel lógico alto ou baixo no pino
int Sensor::sensorTemp_read_bit(gpio_num_t pin) {
    int bit; //Cria uma variavel para receber o valor no nivel lógico
    gpio_set_direction(pin, GPIO_MODE_OUTPUT); //Ajusta o pino como saida
    gpio_set_level(pin, 0); //Ajusta no nivel lógico do pino em 0, baixo
    esp_rom_delay_us(3); //Delay
    gpio_set_direction(pin, GPIO_MODE_INPUT); //Ajusta o pino como entrada
    esp_rom_delay_us(10); //Delay
    bit = gpio_get_level(pin); //Lê o nivel lógico que o sensor enviou para o pino
    esp_rom_delay_us(53); //Delay
    return bit; //Retorna o nivel lógico recebido
}

//Escreve dois bytes para o sensor, com base nos niveis lógicos dos bits que formam o byte
void Sensor::sensorTemp_write_byte(gpio_num_t pin, uint8_t byte) {
    for (int i = 0; i < 8; i++) {//for de 0 a 8 por que são 8 bits que formam um byte
        sensorTemp_write_bit(pin, byte & 0x01);//Escreve bit a bit no pino para que o sensor entenda
        byte >>= 1;//Depois de enviar um bit, desloca para o proximo
    }
}

//Lê os bytes enviados pelo sensor, com base nos niveis lógicos dos bits que formam o byte
uint8_t Sensor::sensorTemp_read_byte(gpio_num_t pin) {
    uint8_t value = 0;//Variavel que vai armazenar o byte recebido
    for (int i = 0; i < 8; i++) {//for de 0 a 8 por que são 8 bits que formam um byte
        int bit = sensorTemp_read_bit(pin);//Lê um bit recebido
        value |= (bit << i);//Coloca o bit na posição para montar o byte
    }
    return value; //Retorna o byte recebido
}