#include <Arduino.h>
const int sensorPin = 36;
const int bombaPin = 14;

int valorSensor;
unsigned long tiempoAnterior = 0;
unsigned long tiempoEspera = 1000; // Ajusta este valor según tus necesidades

void setup() {
  pinMode(sensorPin, INPUT_PULLDOWN);
  pinMode(bombaPin, OUTPUT);

  Serial.begin(115200); // Ajustado a una velocidad de comunicación más alta
}

void loop() {
  valorSensor = analogRead(sensorPin);

  // Imprimir el valor del sensor en el puerto serie
  Serial.print("Valor del sensor: ");
  Serial.println(valorSensor);
  delay(2000);

if (valorSensor >= 3000) {
    digitalWrite(bombaPin, HIGH);

    // Imprimir mensaje indicando que la bomba está encendida
    Serial.println("Bomba encendida");
  } else {
    digitalWrite(bombaPin, LOW);

    // Imprimir mensaje indicando que la bomba está apagada
    Serial.println("Bomba apagada");
  }

  // Espera antes de volver a leer el sensor
  if (millis() - tiempoAnterior >= tiempoEspera) {
    tiempoAnterior = millis();
  }
}
