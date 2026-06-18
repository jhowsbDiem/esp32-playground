// --- Definição dos Pinos ---
const int motorEsq_IN1 = 14; 
const int motorEsq_IN2 = 13; 
const int motorDir_IN3 = 19; 
const int motorDir_IN4 = 23; 

const int pinoC1Esq = 17; 
const int pinoC1Dir = 18; 

// --- Variáveis de Contagem ---
volatile long pulsosEsq = 0;
volatile long pulsosDir = 0;

// --- Parâmetros de Controle ---
const int freq = 500;        
const int resolucao = 8;     
int velocidadeBase = 160;    
float Kp = 2.0;              

// --- Controle de Tempo e Direção ---
unsigned long tempoUltimaInversao = 0;
bool indoParaFrente = true;

// --- Interrupções ---
void IRAM_ATTR countEsq() { pulsosEsq++; }
void IRAM_ATTR countDir() { pulsosDir++; }

// --- Função de Kickstart Inteligente ---
void aplicarKickstart(bool frente) {
  if (frente) {
    ledcWrite(motorEsq_IN2, 0);
    ledcWrite(motorDir_IN4, 0);
    ledcWrite(motorEsq_IN1, 255);
    ledcWrite(motorDir_IN3, 255);
  } else {
    ledcWrite(motorEsq_IN1, 0);
    ledcWrite(motorDir_IN3, 0);
    ledcWrite(motorEsq_IN2, 255);
    ledcWrite(motorDir_IN4, 255);
  }
  delay(80); // O único delay permitido, só para o tranco inicial
}

void setup() {  
  Serial.begin(115200);

  // Agora todos os 4 pinos recebem PWM (Core 3.0+)
  // Isso permite controle de velocidade tanto para frente quanto para trás
  ledcAttach(motorEsq_IN1, freq, resolucao);
  ledcAttach(motorEsq_IN2, freq, resolucao);
  ledcAttach(motorDir_IN3, freq, resolucao);
  ledcAttach(motorDir_IN4, freq, resolucao);

  pinMode(pinoC1Esq, INPUT_PULLUP);
  pinMode(pinoC1Dir, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinoC1Esq), countEsq, RISING);
  attachInterrupt(digitalPinToInterrupt(pinoC1Dir), countDir, RISING);

  Serial.println("Iniciando Loop: 2s Frente / 2s Trás...");
  
  aplicarKickstart(indoParaFrente);
  tempoUltimaInversao = millis(); // Inicia o cronômetro
}

void loop() {
  // --- VERIFICAÇÃO DO CRONÔMETRO (A cada 2 segundos) ---
  if (millis() - tempoUltimaInversao >= 2000) {
    tempoUltimaInversao = millis(); // Reseta o cronômetro
    indoParaFrente = !indoParaFrente; // Inverte a direção

    // Zera os pulsos para o robô não tentar compensar um erro da direção anterior
    pulsosEsq = 0;
    pulsosDir = 0;

    Serial.println(indoParaFrente ? "-> FRENTE" : "<- TRÁS");
    aplicarKickstart(indoParaFrente);
  }

  // --- LÓGICA DE CORREÇÃO (Roda o tempo todo) ---
  long erro = pulsosEsq - pulsosDir;
  int ajuste = (int)(erro * Kp);

  int pwmEsq = velocidadeBase - ajuste;
  int pwmDir = velocidadeBase + ajuste;

  pwmEsq = constrain(pwmEsq, 130, 255);
  pwmDir = constrain(pwmDir, 130, 255);

  // --- APLICAÇÃO DA DIREÇÃO ---
  if (indoParaFrente) {
    ledcWrite(motorEsq_IN2, 0);
    ledcWrite(motorDir_IN4, 0);
    ledcWrite(motorEsq_IN1, pwmEsq);
    ledcWrite(motorDir_IN3, pwmDir);
  } else {
    ledcWrite(motorEsq_IN1, 0);
    ledcWrite(motorDir_IN3, 0);
    // Na marcha ré, injetamos o PWM nos pinos secundários
    ledcWrite(motorEsq_IN2, pwmEsq);
    ledcWrite(motorDir_IN4, pwmDir);
  }

  // Debug a cada 100ms
  static unsigned long tempoAnterior = 0;
  if (millis() - tempoAnterior > 100) {
    tempoAnterior = millis();
    Serial.printf("Modo: %s | E: %ld | D: %ld | PWM_E: %d | PWM_D: %d\n", 
                  indoParaFrente ? "F" : "T", pulsosEsq, pulsosDir, pwmEsq, pwmDir);
  }
}