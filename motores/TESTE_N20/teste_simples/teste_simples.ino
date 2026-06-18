// Definição dos Pinos dos Motores (Ponte H)
const int motorEsq_IN1 = 14;
const int motorEsq_IN2 = 13;
const int motorDir_IN3 = 23;
const int motorDir_IN4 = 19;

// Configurações de PWM
const int freq = 1000;
const int resolucao = 8; // 0 a 255
int velocidadeBase = 180; // Ajuste conforme a força da sua bateria

void setup() {
  Serial.begin(115200);

  // Configura todos os pinos para PWM (Core 3.0+)
  ledcAttach(motorEsq_IN1, freq, resolucao);
  ledcAttach(motorEsq_IN2, freq, resolucao);
  ledcAttach(motorDir_IN3, freq, resolucao);
  ledcAttach(motorDir_IN4, freq, resolucao);

  Serial.println("Carrinho pronto para testes!");
  delay(2000);
}

// --- Funções de Movimentação ---

void frente(int vel) {
  ledcWrite(motorEsq_IN1, vel);
  ledcWrite(motorEsq_IN2, 0);
  ledcWrite(motorDir_IN3, vel);
  ledcWrite(motorDir_IN4, 0);
}

void tras(int vel) {
  ledcWrite(motorEsq_IN1, 0);
  ledcWrite(motorEsq_IN2, vel);
  ledcWrite(motorDir_IN3, 0);
  ledcWrite(motorDir_IN4, vel);
}

void esquerda(int vel) {
  ledcWrite(motorEsq_IN1, 0);
  ledcWrite(motorEsq_IN2, vel); // Roda esquerda para trás
  ledcWrite(motorDir_IN3, vel); // Roda direita para frente
  ledcWrite(motorDir_IN4, 0);
}

void direita(int vel) {
  ledcWrite(motorEsq_IN1, vel); // Roda esquerda para frente
  ledcWrite(motorEsq_IN2, 0);
  ledcWrite(motorDir_IN3, 0);   // Roda direita para trás
  ledcWrite(motorDir_IN4, vel);
}

void parar() {
  ledcWrite(motorEsq_IN1, 0);
  ledcWrite(motorEsq_IN2, 0);
  ledcWrite(motorDir_IN3, 0);
  ledcWrite(motorDir_IN4, 0);
}

void loop() {
  // Exemplo de sequência de movimentos para teste:
  
  Serial.println("Frente...");
  frente(velocidadeBase);
  delay(2000);

  Serial.println("Parar...");
  parar();
  delay(1000);

  Serial.println("Direita...");
  direita(velocidadeBase);
  delay(1000);

  Serial.println("Esquerda...");
  esquerda(velocidadeBase);
  delay(1000);

  Serial.println("Parar...");
  parar();
  delay(3000);
}