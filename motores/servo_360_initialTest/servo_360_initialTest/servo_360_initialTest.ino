#include <Arduino.h>
#include <ESP32Servo.h>

#include "direcoes.h"



void setup() {
  Serial.begin(115200);
  setupServo();
}

void loop() {

frente();
parar();

while(true);

}