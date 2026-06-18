#ifndef DIRECOES_H

extern const int PINO_SERVO_ESQ;
extern const int PINO_SERVO_DIR;

extern float multiplicadorDIR; 
extern float multiplicadorESQ; 

extern Servo servoEsq;
extern Servo servoDir;

extern int stopEsq; 
extern int stopDir; 

void setupServo();

void parar();
void frente(int time = 3200);
void tras(int time = 3200);
void esquerda();
void direita();

#endif