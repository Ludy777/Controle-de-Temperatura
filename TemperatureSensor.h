#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class TemperatureSensor {
public:
    TemperatureSensor(gpio_num_t pin);
    bool init();
    float readTemperature();

private:
    gpio_num_t _pin;

    void setPinInput();
    void setPinOutput();
    void writeBit(int bit);
    int readBit();
    void writeByte(uint8_t data);
    uint8_t readByte();
    bool resetPulse();

    static constexpr const char* TAG = "TemperatureSensor";
};

#endif
