#include "FanController.h"
#include "TemperatureSensor.h"

extern "C" void app_main(void) {
    TemperatureSensor tempSensor(GPIO_NUM_4);
    FanController fan(GPIO_NUM_23, &tempSensor);

    fan.begin();

    while (true) {
        fan.update();
        vTaskDelay(pdMS_TO_TICKS(100)); // Atualiza a cada segundo
    }
}
