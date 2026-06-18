/**
 * @file Teste_VL53L0X_Shutdown.ino
 * @brief Sketch de teste isolado para o sensor de distância VL53L0X.
 * @details Demonstra como usar o pino XSHUT para colocar o sensor em modo 
 * de baixo consumo (Hardware Standby) e como acordá-lo para fazer leituras.
 * Ideal para medir a economia de energia.
 */

#include "Wire.h"
#include "Adafruit_VL53L0X.h"

// =================================================================
// CONFIGURAÇÃO DA FIAÇÃO
// =================================================================
// | ESP32 Pin | Componente                 | VL53L0X Pin          |
// |-----------|----------------------------|----------------------|
// |   3.3V    | Alimentação                | VCC / VIN            |
// |    GND    | Terra                      | GND                  |
// |  GPIO 22  | I2C Clock                  | SCL                  |
// |  GPIO 21  | I2C Data                   | SDA                  |
// |  GPIO 26  | Controle de Shutdown       | XSHUT                |
// -----------------------------------------------------------------

// Pino para controlar o modo Shutdown do sensor
#define XSHUT_PIN 26

// Cria o objeto do sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// =================================================================
// SETUP
// =================================================================
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Iniciando teste do VL53L0X com controle de energia (XSHUT)...");

  // Configura o pino de shutdown como saída
  pinMode(XSHUT_PIN, OUTPUT);
  
  // Acorda o sensor para a inicialização
  digitalWrite(XSHUT_PIN, HIGH);
  delay(150); // O datasheet recomenda um tempo para o sensor estabilizar

  Serial.println("Inicializando o sensor...");
  if (!lox.begin()) {
    Serial.println(F("ERRO: Não foi possível inicializar o VL53L0X. Verifique a fiação I2C."));
    while (1);
  }

  Serial.println(F("Sensor inicializado com sucesso!"));
  Serial.println(F(">>> O sensor está ATIVO. Meça a corrente em modo de LEITURA agora. <<<"));
  Serial.println(F("-----------------------------------------"));
  Serial.println(F("Envie comandos pelo Monitor Serial:"));
  Serial.println(F(" 'm' -> para fazer uma medição (Measure)"));
  Serial.println(F(" 's' -> para colocar o sensor para dormir (Sleep)"));
  Serial.println(F(" 'w' -> para acordar o sensor (Wake)"));
  Serial.println(F("-----------------------------------------"));
}

// =================================================================
// LOOP
// =================================================================
void loop() {
  // Verifica se um comando foi enviado pelo Monitor Serial
  if (Serial.available()) {
    char command = Serial.read();

    switch (command) {
      case 'm':
        Serial.println(F("\nComando recebido: Medir"));
        fazerMedicao();
        break;

      case 's':
        Serial.println(F("\nComando recebido: Sleep"));
        digitalWrite(XSHUT_PIN, LOW); // Coloca o pino em LOW para desligar o sensor
        Serial.println(F(">>> Sensor em modo Shutdown (dormindo). Meça a corrente AGORA. <<<"));
        break;

      case 'w':
        Serial.println(F("\nComando recebido: Wake"));
        digitalWrite(XSHUT_PIN, HIGH); // Coloca o pino em HIGH para acordar
        delay(150);
        Serial.println(F("Reinicializando o sensor (lox.begin)..."));
        if (!lox.begin()) {
          Serial.println(F("ERRO: Falha ao reinicializar o sensor."));
        } else {
          Serial.println(F(">>> Sensor acordado e pronto. Meça a corrente em modo OCIOSO agora. <<<"));
        }
        break;
    }
  }
}

/**
 * @brief Função para fazer uma única medição de distância.
 */
void fazerMedicao() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Lendo distância... ");
  lox.rangingTest(&measure, false); // o "false" desativa o modo debug verboso

  if (measure.RangeStatus != 4) {  // Se a leitura for válida
    Serial.print("Distância (mm): "); 
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Fora de alcance (out of range)");
  }
}