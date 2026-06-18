#include <ESP32Servo.h>

// ========================================
// CONFIGURAÇÕES DOS SERVOS
// ========================================
// Pinos dos servo motores
const int SERVO_PINS[5] = {12, 15, 28, 26, 25}; // Polegar, Indicador, Médio, Anelar, Mínimo

// Objetos Servo
Servo servos[5];

// Nomes dos dedos para debug
const char* fingerNames[5] = {"POLEGAR", "INDICADOR", "MÉDIO", "ANELAR", "MÍNIMO"};

// Posições atuais dos servos
int currentPositions[5] = {90, 90, 90, 90, 90}; // Todos começam em 90°

// ========================================
// FUNÇÕES AUXILIARES
// ========================================

void printHelp() {
  Serial.println("\n🤖 === CONTROLE DE MÃO ROBÓTICA ===");
  Serial.println("📋 Comandos disponíveis:");
  Serial.println("   [número_servo] [ângulo]");
  Serial.println("");
  Serial.println("📍 Servos disponíveis:");
  for (int i = 0; i < 5; i++) {
    Serial.printf("   %d = %s (atual: %d°)\n", i+1, fingerNames[i], currentPositions[i]);
  }
  Serial.println("");
  Serial.println("💡 Exemplos:");
  Serial.println("   1 180    → Move POLEGAR para 180°");
  Serial.println("   2 30     → Move INDICADOR para 30°");
  Serial.println("   3 90     → Move MÉDIO para 90°");
  Serial.println("   help     → Mostra esta ajuda");
  Serial.println("   status   → Mostra posições atuais");
  Serial.println("   test     → Testa todos os servos");
  Serial.println("   reset    → Volta todos para 90°");
  Serial.println("=====================================\n");
}

void printStatus() {
  Serial.println("\n📊 === STATUS ATUAL DOS SERVOS ===");
  for (int i = 0; i < 5; i++) {
    Serial.printf("   %d - %s: %d°\n", i+1, fingerNames[i], currentPositions[i]);
  }
  Serial.println("==================================\n");
}

void moveServo(int servoIndex, int angle) {
  // Verificar se o índice é válido
  if (servoIndex < 0 || servoIndex >= 5) {
    Serial.printf("❌ Erro: Servo %d não existe! Use 1-5\n", servoIndex + 1);
    return;
  }
  
  // Limitar ângulo entre 0 e 180
  angle = constrain(angle, 0, 180);
  
  // Mover servo
  servos[servoIndex].write(angle);
  currentPositions[servoIndex] = angle;
  
  Serial.printf("✅ %s movido para %d°\n", fingerNames[servoIndex], angle);
}

void testAllServos() {
  Serial.println("🧪 Testando todos os servos...");
  
  // Teste 1: Mover todos para 0°
  Serial.println("   → Movendo todos para 0°");
  for (int i = 0; i < 5; i++) {
    moveServo(i, 0);
    delay(200);
  }
  delay(1000);
  
  // Teste 2: Mover todos para 180°
  Serial.println("   → Movendo todos para 180°");
  for (int i = 0; i < 5; i++) {
    moveServo(i, 180);
    delay(200);
  }
  delay(1000);
  
  // Teste 3: Mover todos para 90° (posição neutra)
  Serial.println("   → Voltando todos para 90°");
  for (int i = 0; i < 5; i++) {
    moveServo(i, 90);
    delay(200);
  }
  
  Serial.println("✅ Teste concluído!");
}

void resetAllServos() {
  Serial.println("🔄 Resetando todos os servos para 90°...");
  for (int i = 0; i < 5; i++) {
    moveServo(i, 90);
    delay(100);
  }
  Serial.println("✅ Reset concluído!");
}

void processSerialCommand(String command) {
  command.trim(); // Remove espaços extras
  command.toLowerCase(); // Converte para minúsculo
  
  // Comandos especiais
  if (command == "help" || command == "h") {
    printHelp();
    return;
  }
  
  if (command == "status" || command == "s") {
    printStatus();
    return;
  }
  
  if (command == "test" || command == "t") {
    testAllServos();
    return;
  }
  
  if (command == "reset" || command == "r") {
    resetAllServos();
    return;
  }
  
  // Comando de movimento: [servo] [ângulo]
  int spaceIndex = command.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("❌ Formato inválido! Use: [servo] [ângulo]");
    Serial.println("💡 Digite 'help' para ver os comandos");
    return;
  }
  
  String servoStr = command.substring(0, spaceIndex);
  String angleStr = command.substring(spaceIndex + 1);
  
  int servoNum = servoStr.toInt();
  int angle = angleStr.toInt();
  
  // Verificar se os valores são válidos
  if (servoNum < 1 || servoNum > 5) {
    Serial.println("❌ Número do servo inválido! Use 1-5");
    return;
  }
  
  if (angle < 0 || angle > 180) {
    Serial.println("❌ Ângulo inválido! Use 0-180");
    return;
  }
  
  // Mover servo (converter para índice 0-4)
  moveServo(servoNum - 1, angle);
}

// ========================================
// CONFIGURAÇÃO INICIAL
// ========================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n🤖 INICIANDO TESTE DE MÃO ROBÓTICA");
  Serial.println("====================================");
  
  // Configurar servos
  Serial.println("🔧 Configurando servos...");
  for (int i = 0; i < 5; i++) {
    servos[i].attach(SERVO_PINS[i]);
    Serial.printf("   ✅ %s conectado no pino %d\n", fingerNames[i], SERVO_PINS[i]);
  }
  
  // Posicionar todos em 90° (posição inicial)
  Serial.println("\n📍 Posicionando servos em 90°...");
  for (int i = 0; i < 5; i++) {
    servos[i].write(90);
    delay(100);
  }
  
  delay(1000);
  Serial.println("✅ Configuração concluída!");
  
  // Mostrar ajuda inicial
  printHelp();
  
  Serial.println("🎮 Sistema pronto! Digite um comando:");
}

// ========================================
// LOOP PRINCIPAL
// ========================================
void loop() {
  // Verificar se há dados no Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processSerialCommand(command);
  }
  
  delay(10); // Pequeno delay para não sobrecarregar
}

// ========================================
// FUNÇÕES EXTRAS PARA DEBUG
// ========================================

// Função para mover servo com animação suave
void moveServoSmooth(int servoIndex, int targetAngle, int delayMs = 15) {
  if (servoIndex < 0 || servoIndex >= 5) return;
  
  int currentAngle = currentPositions[servoIndex];
  int step = (targetAngle > currentAngle) ? 1 : -1;
  
  Serial.printf("🎬 Movendo %s suavemente de %d° para %d°\n", 
                fingerNames[servoIndex], currentAngle, targetAngle);
  
  while (currentAngle != targetAngle) {
    currentAngle += step;
    servos[servoIndex].write(currentAngle);
    currentPositions[servoIndex] = currentAngle;
    delay(delayMs);
  }
  
  Serial.printf("✅ %s chegou em %d°\n", fingerNames[servoIndex], targetAngle);
}

// Função para sequência de demonstração
void demoSequence() {
  Serial.println("🎭 Iniciando sequência de demonstração...");
  
  // Fechar mão gradualmente
  Serial.println("   → Fechando mão...");
  for (int angle = 90; angle >= 0; angle -= 10) {
    for (int i = 0; i < 5; i++) {
      moveServo(i, angle);
    }
    delay(200);
  }
  
  delay(1000);
  
  // Abrir mão gradualmente
  Serial.println("   → Abrindo mão...");
  for (int angle = 0; angle <= 180; angle += 10) {
    for (int i = 0; i < 5; i++) {
      moveServo(i, angle);
    }
    delay(200);
  }
  
  delay(1000);
  
  // Voltar para posição neutra
  Serial.println("   → Voltando para posição neutra...");
  for (int i = 0; i < 5; i++) {
    moveServo(i, 90);
    delay(100);
  }
  
  Serial.println("✅ Demonstração concluída!");
}