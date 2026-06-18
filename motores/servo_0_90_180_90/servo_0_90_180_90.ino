#include <ESP32Servo.h>

int pinoServo = 27;    // Pino PWM do servo
Servo servo;

const int botao = 14;

void setup() {

    servo.attach(pinoServo);
    Serial.begin(115200);

    pinMode(botao, INPUT_PULLUP);

}

void loop() {

if (digitalRead(botao) == LOW) {
Serial.println("botão pressionado");
servoMove();
}

}

void servoMove() {
    
    servo.write(0);
 delay(1000);
servo.write(90);
delay(1000);
 servo.write(180);
 delay(1000);
 servo.write(90);
 delay(1000);

}