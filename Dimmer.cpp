#include "Dimmer.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

//Cria a classe dimmer
Dimmer::Dimmer(gpio_num_t pin) {
    triacPin = pin;
    potPwm = 0;
    delayMicrosegundos = 8000;
    setPwm(0);
}

//Inicia a classe
void Dimmer::inicio() {
    gpio_set_direction(triacPin, GPIO_MODE_OUTPUT);
    gpio_set_level(triacPin, 0);
    ESP_LOGI("", "Dimmer iniciado no pino %d", triacPin);
}

//Ajusta o delay do pwm
void Dimmer::setPwm(int pwm) {
    if (pwm < 0) pwm = 0;
    if (pwm > 100) pwm = 100;
    potPwm = pwm;
    delayMicrosegundos = 8000 - (pwm * 80);

    ESP_LOGI("","PWM lampada: %d", pwm);
}

//Aplica o delay do pwm no pino
void Dimmer::onZeroCross() {
    esp_rom_delay_us(delayMicrosegundos);
    gpio_set_level(triacPin, 1);
    esp_rom_delay_us(100);
    gpio_set_level(triacPin, 0);
}

//Destrutor da classe
Dimmer::~Dimmer(){
    gpio_set_level(triacPin, 0);
}