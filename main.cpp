#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4   // Pino de dados do DS18B20
#define FAN_PIN 23       // Pino da ventoinha

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

bool fanOn = false;

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  sensors.begin();
  Serial.println("Sistema de controle iniciado...");
}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" °C");

  if (!fanOn && temp > 26.2) {
    digitalWrite(FAN_PIN, HIGH);
    fanOn = true;
    Serial.println("⚙️ Ventoinha LIGADA");
  } 
  else if (fanOn && temp < 25.8) {
    digitalWrite(FAN_PIN, LOW);
    fanOn = false;
    Serial.println("💤 Ventoinha DESLIGADA");
  }

  delay(1000); // Atualiza a cada segundo
}
