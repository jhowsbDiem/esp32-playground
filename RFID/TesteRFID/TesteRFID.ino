/**
 * @file Teste_RFID_Sleep.ino
 * @brief Sketch de teste isolado para o módulo MFRC522 (RFID).
 * @details Este código permite testar o funcionamento normal (buscando por cartões)
 * e o modo de baixo consumo (Soft Power Down), com feedback via Monitor Serial.
 * Ideal para verificar a economia de energia do modo sleep.
 * * @author DevHelper Gem
 * @date   16/09/2025
 */

// =================================================================
// BIBLIOTECAS
// =================================================================
#include <SPI.h>
#include <MFRC522.h>

// =================================================================
// CONFIGURAÇÃO DA FIAÇÃO (Pinos padrão do seu projeto)
// =================================================================
//
// DIAGRAMA DE FIAÇÃO:
// -----------------------------------------------------------------
// | ESP32 Pin | Componente                 | MFRC522 Pin          |
// |-----------|----------------------------|----------------------|
// |   3.3V    | Alimentação                | 3.3V                 |
// |    GND    | Terra                      | GND                  |
// |  GPIO 5   | Reset                      | RST                  |
// |  GPIO 4   | Chip Select (Slave Select) | SDA (CS/SS)          |
// |  GPIO 23  | Master Out Slave In        | MOSI                 |
// |  GPIO 34  | Master In Slave Out        | MISO                 |
// |  GPIO 18  | Serial Clock               | SCK                  |
// -----------------------------------------------------------------
#define SS_PIN  4  // SDA / CS
#define RST_PIN 5  // RST

// Cria o objeto MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Variável para controlar se o módulo está ativo ou dormindo
bool rfidAtivo = true;

// =================================================================
// SETUP
// =================================================================
void setup() {
  // Inicia a comunicação com o computador (Monitor Serial)
  Serial.begin(115200);
  while (!Serial); // Espera o Serial estar pronto
  
  Serial.println(F("Iniciando teste do MFRC522 (RFID)..."));

  // Inicia o barramento SPI
  SPI.begin(18, 34, 23, 4); // SCK, MISO, MOSI, SS

  // Inicia o módulo MFRC522
  mfrc522.PCD_Init();
  
  // Teste de verificação: Tenta ler a versão do firmware do chip.
  // Se não conseguir, há um problema de fiação ou hardware.
  byte firmwareVersion = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (firmwareVersion == 0x00 || firmwareVersion == 0xFF) {
    Serial.println(F("ERRO: Módulo MFRC522 não encontrado."));
    Serial.println(F("Verifique a fiação SPI (SCK, MISO, MOSI, SS, RST)."));
    while (true); // Trava a execução
  }

  Serial.println(F("Módulo MFRC522 inicializado com sucesso!"));
  Serial.print(F("Versão do Firmware: 0x"));
  Serial.println(firmwareVersion, HEX);
  Serial.println(F("-----------------------------------------"));
  Serial.println(F("O módulo está ATIVO, buscando por cartões."));
  Serial.println(F(">>> Você pode medir a corrente do modo de BUSCA (POLLING) agora. <<<"));
  Serial.println(F("-----------------------------------------"));
  Serial.println(F("Envie comandos pelo Monitor Serial:"));
  Serial.println(F(" 's' -> para colocar o módulo para dormir (Sleep)"));
  Serial.println(F(" 'w' -> para acordar o módulo (Wake)"));
  Serial.println(F("-----------------------------------------"));
}

// =================================================================
// LOOP
// =================================================================
void loop() {
  // Se o módulo estiver ativo, ele fica procurando por cartões
  if (rfidAtivo) {
    // Procura por um novo cartão
    if (mfrc522.PICC_IsNewCardPresent()) {
      // Se encontrou, lê o UID (número de série)
      if (mfrc522.PICC_ReadCardSerial()) {
        Serial.print("Cartão detectado! UID: ");
        // Imprime o UID em hexadecimal
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.println();
        
        // Libera o cartão para a próxima leitura
        mfrc522.PICC_HaltA();
        delay(1000); // Pausa para não ler o mesmo cartão várias vezes
      }
    }
  }

  // Verifica se um comando foi enviado pelo Monitor Serial
  if (Serial.available()) {
    char command = Serial.read();

    switch (command) {
      case 's':
        Serial.println(F("\nComando recebido: Sleep"));
        mfrc522.PCD_SoftPowerDown(); // Coloca o chip em modo de baixo consumo
        rfidAtivo = false;
        Serial.println(F(">>> Módulo dormindo. Meça a corrente do modo SLEEP agora. <<<"));
        break;

      case 'w':
        Serial.println(F("\nComando recebido: Wake"));
        mfrc522.PCD_Init(); // Para acordar, é preciso reinicializar o módulo
        rfidAtivo = true;
        Serial.println(F(">>> Módulo acordado. Meça a corrente do modo de BUSCA novamente. <<<"));
        Serial.println(F("Aproxime um cartão para confirmar."));
        break;
    }
  }
}