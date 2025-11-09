#include "TemperatureSensor.h"
#include "esp_rom_sys.h" // Para esp_rom_delay_us()

static const char *TAG = "TemperatureSensor";

// ---- Configuração do pino ----
TemperatureSensor::TemperatureSensor(gpio_num_t dataPin) {
    pin = dataPin;
    lastTemperature = 25.0; // valor inicial
}

void TemperatureSensor::begin() {
    gpio_set_direction(pin, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    ESP_LOGI(TAG, "Sensor DS18B20 inicializado no pino %d", pin);
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
float TemperatureSensor::readRawTemperature() {
    onewire_reset(pin);
    onewire_write_byte(pin, 0xCC); // Skip ROM
    onewire_write_byte(pin, 0x44); // Start temperature conversion

    vTaskDelay(pdMS_TO_TICKS(750)); // tempo de conversão do DS18B20

    onewire_reset(pin);
    onewire_write_byte(pin, 0xCC); // Skip ROM
    onewire_write_byte(pin, 0xBE); // Read Scratchpad

    uint8_t tempLSB = onewire_read_byte(pin);
    uint8_t tempMSB = onewire_read_byte(pin);

    int16_t raw = (tempMSB << 8) | tempLSB;
    return raw / 16.0f;
}

// ---- Filtro simples para estabilidade ----
float TemperatureSensor::readCelsius() {
    float newTemp = readRawTemperature();

    // filtro de suavização exponencial (média móvel leve)
    lastTemperature = (0.8f * lastTemperature) + (0.2f * newTemp);

    ESP_LOGI("","Temp lida: %.2f °C", lastTemperature);
    return lastTemperature;
}
