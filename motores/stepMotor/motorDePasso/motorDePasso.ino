#include <Wire.h>

#define PCF_ADDR 0x20

float rpmDesejado = 20.0; 
const long passosPorVolta = 4096; 
unsigned long delayMicros; 

byte passos[] = {
  B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001
};

int indiceM1 = 0;
int indiceM2 = 0;

void enviarDados(int dado) {
  Wire.beginTransmission(PCF_ADDR);
  Wire.write(dado);                 
  Wire.endTransmission();
}

enum sentido {
  FRENTE    = 1,
  TRAS      = 2,
  DIREITA   = 3,
  ESQUERDA  = 4,
  GIRAR     = 5
};

// =================================================================
// ⚙️ MOTOR
// =================================================================
void executarPassos (int passosDesejados, int direcao) {

  for (int i = 0; i < passosDesejados; i++) {

    if (direcao == FRENTE) {
      indiceM1++; if (indiceM1 > 7) indiceM1 = 0;
      indiceM2++; if (indiceM2 > 7) indiceM2 = 0;
    } 
    else if (direcao == TRAS) {
      indiceM1--; if (indiceM1 < 0) indiceM1 = 7;
      indiceM2--; if (indiceM2 < 0) indiceM2 = 7;
    } 
    else if (direcao == ESQUERDA) {
      indiceM1--; if (indiceM1 < 0) indiceM1 = 7;
      indiceM2++; if (indiceM2 > 7) indiceM2 = 0;
    } 
    else if (direcao == DIREITA) {
      indiceM1++; if (indiceM1 > 7) indiceM1 = 0;
      indiceM2--; if (indiceM2 < 0) indiceM2 = 7;
    } else if (direcao == GIRAR) {
      indiceM1--; if (indiceM1 < 0) indiceM1 = 7;
      indiceM2++; if (indiceM2 > 7) indiceM2 = 0;
    }

    byte estadoM1 = passos[indiceM1];
    byte estadoM2 = passos[indiceM2];
    byte dadoFinal = (estadoM2 << 4) | estadoM1;
    
    enviarDados(dadoFinal);
    delayMicroseconds(delayMicros);

  }
}

// =================================================================
// COMANDOS DOS MOTORES
// =================================================================

// fazem os motores andarem já na direção correta.
// sendo necessario fornecer somente os graus "distancia" que robo ira andar
void andarRoboFrente(float graus_da_roda) {
  int passosDesejados = (passosPorVolta * graus_da_roda) / 360;
  executarPassos(passosDesejados, FRENTE);
}

void andarRoboTras(float graus_da_roda) {
  int passosDesejados = (passosPorVolta * graus_da_roda) / 360;
  executarPassos(passosDesejados, TRAS);
}

void andarRoboDireita(float graus_da_roda) {
  int passosDesejados = (passosPorVolta * graus_da_roda) / 360;
  executarPassos(passosDesejados, DIREITA);
}

void andarRoboEsquerda(float graus_da_roda) {
  int passosDesejados = (passosPorVolta * graus_da_roda) / 360;
  executarPassos(passosDesejados, ESQUERDA);
}

void girarRobo(float graus_da_roda) {
  int passosDesejados = (passosPorVolta * graus_da_roda) / 360;
  executarPassos(passosDesejados, GIRAR);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); 
  Wire.setClock(400000);

  // Calcula o delay necessário para atingir o RPM
  delayMicros = 60000000UL / (passosPorVolta * rpmDesejado);
  
  Serial.println("Iniciando sequencia de movimentos...");
  delay(2000); // Espera 2 segundos antes de começar
}

void loop() {
  // --- A SUA SEQUÊNCIA DE MOVIMENTOS ---
  
  andarRoboFrente(360);
  delay(500);

  andarRoboFrente(360);
  delay(500);

  andarRoboEsquerda(205);
  delay(500);

  andarRoboFrente(360);
  delay(500);

  andarRoboFrente(360);
  delay(500);

  andarRoboTras(360);
  delay(500);

  andarRoboTras(360);
  delay(500);

  andarRoboDireita(205);
  delay(500);

  andarRoboTras(360);
  delay(500);

  andarRoboTras(360);
  delay(500);
  
  // girarRobo(818);
  // delay(500); 

  // andarRoboTras(360);
  
  // Pausa longa de 5 segundos antes de repetir todo o ciclo desde o início
  Serial.println("Fim do ciclo. Reiniciando em 5 segundos...");
  delay(2000); 
}