#include <Arduino.h>
#include <ESP32Servo.h>

#include "direcoes.h"

// Definindo os pinos
const int PINO_SERVO_ESQ = 12;
const int PINO_SERVO_DIR = 15;

Servo servoEsq;
Servo servoDir;

// --- AJUSTE DE CALIBRAÇÃO (O SEGREDO DA PRECISÃO) ---
// Tente achar o valor exato onde cada motor fica TOTALMENTE parado.
// Geralmente é perto de 1500.
int stopEsq = 1500; 
int stopDir = 1500; 

float multiplicadorDIR = 2.0; // teste para igualar os dois servos 
float multiplicadorESQ = 2.0; 

void setupServo(){
   // Aloca timers para o ESP32 (importante para estabilidade)
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  servoEsq.setPeriodHertz(50);
  servoDir.setPeriodHertz(50);

  servoEsq.attach(PINO_SERVO_ESQ, 500, 2500);
  servoDir.attach(PINO_SERVO_DIR, 500, 2500);

  // Garante que comecem parados
  servoEsq.writeMicroseconds(stopEsq);
  servoDir.writeMicroseconds(stopDir);
  
  delay(2000); // Espera 2 segundos antes de começar
}

void parar() {

  Serial.println("Parando...");

  servoEsq.writeMicroseconds(stopEsq); // Stop value set at 1500
  servoDir.writeMicroseconds(stopDir);

  delay(1000);

}

void frente(int time) {
  Serial.println("Andando para frente...");
  
  // Para ir para frente, um motor gira no sentido horário e outro anti-horário
  // devido à posição espelhada deles no chassi.
  // Ajuste o valor '200' para aumentar ou diminuir a velocidade.
  servoEsq.writeMicroseconds(stopEsq + (100 * multiplicadorESQ)); 
  servoDir.writeMicroseconds(stopDir - (100 * multiplicadorDIR)); 

  delay(time); // walk duration ( switch to ideal distance )
}

void tras(int time) {
  Serial.println("Andando para trás...");

  servoEsq.writeMicroseconds(stopEsq - (100 * multiplicadorESQ)); 
  servoDir.writeMicroseconds(stopDir + (100 * multiplicadorDIR)); 

  delay(time);
}

void esquerda() {
  Serial.println("Andando para esquerda... ");

  servoEsq.writeMicroseconds(stopEsq + (100 * multiplicadorESQ));
  servoDir.writeMicroseconds(stopDir + (100 * multiplicadorDIR));

  delay(690);
}

void direita() {
  Serial.println("Andando para direita... ");

  servoEsq.writeMicroseconds(stopEsq - (100 * multiplicadorESQ));
  servoDir.writeMicroseconds(stopDir - (100 * multiplicadorDIR));

  delay(720);
}
