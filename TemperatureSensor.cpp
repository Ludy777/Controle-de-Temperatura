#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(gpio_num_t pin) : _pin(pin) {}

bool TemperatureSensor::init() {
    gpio_set_direction(_pin, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(_pin, GPIO_PULLUP_ONLY);
    return true;
}

void TemperatureSensor::setPinInput() {
    gpio_set_direction(_pin, GPIO_MODE_INPUT);
}

void TemperatureSensor::setPinOutput() {
    gpio_set_direction(_pin, GPIO_MODE_OUTPUT_OD);
}

bool TemperatureSensor::resetPulse() {
    setPinOutput();
    gpio_set_level(_pin, 0);
    esp_rom_delay_us(480);
    setPinInput();
    esp_rom_delay_us(70);

    int presence = gpio_get_level(_pin);
    esp_rom_delay_us(410);

    return (presence == 0); // 0 = DS18B20 presente
}

void TemperatureSensor::writeBit(int bit) {
    setPinOutput();
    gpio_set_level(_pin, 0);
    if (bit) {
        esp_rom_delay_us(6);
        setPinInput();
        esp_rom_delay_us(64);
    } else {
        esp_rom_delay_us(60);
        setPinInput();
        esp_rom_delay_us(10);
    }
}

int TemperatureSensor::readBit() {
    int bit;
    setPinOutput();
    gpio_set_level(_pin, 0);
    esp_rom_delay_us(6);
    setPinInput();
    esp_rom_delay_us(9);
    bit = gpio_get_level(_pin);
    esp_rom_delay_us(55);
    return bit;
}

void TemperatureSensor::writeByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        writeBit(data & 0x01);
        data >>= 1;
    }
}

uint8_t TemperatureSensor::readByte() {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        value >>= 1;
        if (readBit())
            value |= 0x80;
    }
    return value;
}

float TemperatureSensor::readTemperature() {
    if (!resetPulse()) {
        ESP_LOGE(TAG, "Sensor não detectado!");
        return -127.0f;
    }

    writeByte(0xCC); // SKIP ROM
    writeByte(0x44); // CONVERT T

    vTaskDelay(pdMS_TO_TICKS(750)); // aguarda conversão (máx 12 bits)

    if (!resetPulse()) {
        ESP_LOGE(TAG, "Erro ao reiniciar após conversão!");
        return -127.0f;
    }

    writeByte(0xCC); // SKIP ROM
    writeByte(0xBE); // READ SCRATCHPAD

    uint8_t temp_lsb = readByte();
    uint8_t temp_msb = readByte();

    int16_t temp_read = (temp_msb << 8) | temp_lsb;
    float temp_celsius = temp_read / 16.0f;

    return temp_celsius;
}
