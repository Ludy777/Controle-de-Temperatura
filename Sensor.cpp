#include "Sensor.h"
#include "esp_rom_sys.h" // Para esp_rom_delay_us()

static const char *TAG = "Sensor";

// ---- Configuração do pino ----
Sensor::Sensor(gpio_num_t dataPin) {
    pino = dataPin;
    ultimaTemp = 25.0; // valor inicial
}

void Sensor::inicio() {
    gpio_set_direction(pino, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pino, GPIO_PULLUP_ONLY);
    ESP_LOGI(TAG, "Sensor DS18B20 inicializado no pino %d", pino);
}

// ---- Funções auxiliares do protocolo OneWire ----
static void onewire_reset(gpio_num_t pin) {
    gpio_set_level(pin, 0);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    esp_rom_delay_us(480);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    esp_rom_delay_us(480);
}

static void onewire_write_bit(gpio_num_t pin, int bit) {
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

static int onewire_read_bit(gpio_num_t pin) {
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

static void onewire_write_byte(gpio_num_t pin, uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        onewire_write_bit(pin, byte & 0x01);
        byte >>= 1;
    }
}

static uint8_t onewire_read_byte(gpio_num_t pin) {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        int bit = onewire_read_bit(pin);
        value |= (bit << i);
    }
    return value;
}

// ---- Leitura DS18B20 ----
float Sensor::leituraTemp() {
    onewire_reset(pino);
    onewire_write_byte(pino, 0xCC); // Skip ROM
    onewire_write_byte(pino, 0x44); // Start temperature conversion

    vTaskDelay(pdMS_TO_TICKS(750)); // tempo de conversão do DS18B20

    onewire_reset(pino);
    onewire_write_byte(pino, 0xCC); // Skip ROM
    onewire_write_byte(pino, 0xBE); // Read Scratchpad

    uint8_t tempLSB = onewire_read_byte(pino);
    uint8_t tempMSB = onewire_read_byte(pino);

    int16_t raw = (tempMSB << 8) | tempLSB;
    return raw / 16.0f;
}

// ---- Filtro simples para estabilidade ----
float Sensor::lerCelsius() {
    float newTemp = leituraTemp();

    // filtro de suavização exponencial (média móvel leve)
    ultimaTemp = (0.8f * ultimaTemp) + (0.2f * newTemp);

    ESP_LOGI("","Temp lida: %.2f °C", ultimaTemp);
    return ultimaTemp;
}

Sensor::~Sensor(){
    ultimaTemp = 0;
}