/**
 * @file Teste_DFPlayer_Sleep.ino
 * @brief Sketch de teste isolado para o módulo DFPlayer Mini.
 * @details Este código permite testar as funções de play, sleep (dormir) e 
 * start (acordar) do módulo, com feedback via Monitor Serial.
 * É ideal para verificar o funcionamento e medir o consumo de 
 * corrente em diferentes estados.
 * * @author DevHelper Gem (baseado na biblioteca DFRobot)
 * @date   16/09/2025
 */

// =================================================================
// BIBLIOTECAS
// =================================================================
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// =================================================================
// CONFIGURAÇÃO DA FIAÇÃO (Altere os pinos se necessário)
// =================================================================
// Conecte o pino RX do DFPlayer ao pino TX do ESP32 (GPIO 17)
// Conecte o pino TX do DFPlayer ao pino RX do ESP32 (GPIO 16) através de um resistor de 1K ohm.
//
// DIAGRAMA DE FIAÇÃO:
// ----------------------------------------------------
// | ESP32 Pin | Componente                 | Conexão  |
// |-----------|----------------------------|----------|
// |   VIN/5V  | DFPlayer Pin VCC           | VCC (5V) |
// |    GND    | DFPlayer Pin GND           | GND      |
// |  GPIO 16  | Resistor 1K -> DFPlayer TX | RX2      |
// |  GPIO 17  | DFPlayer Pin RX            | TX2      |
// |    -      | DFPlayer Pin SPK_1         | Alto-falante + |
// |    -      | DFPlayer Pin SPK_2         | Alto-falante - |
// ----------------------------------------------------

// Pinos para a comunicação Serial por Software
const int pinoRxDFPlayer = 16;
const int pinoTxDFPlayer = 17;

// Cria os objetos
SoftwareSerial mySoftwareSerial(pinoRxDFPlayer, pinoTxDFPlayer); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// =================================================================
// PREPARAÇÃO DO CARTÃO SD (MUITO IMPORTANTE!)
// =================================================================
// 1. Formate o cartão MicroSD como FAT16 ou FAT32.
// 2. Crie uma pasta chamada "mp3" na raiz do cartão.
// 3. Coloque seus arquivos de áudio DENTRO da pasta "mp3".
// 4. Renomeie os arquivos para "0001.mp3", "0002.mp3", etc.

void setup() {
  // Inicia a comunicação com o computador (Monitor Serial)
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Iniciando teste do DFPlayer Mini..."));

  // Inicia a comunicação com o DFPlayer
  mySoftwareSerial.begin(9600);

   // Tenta inicializar o DFPlayer
   if (!myDFPlayer.begin(mySoftwareSerial)) {
     Serial.println(F("ERRO: Não foi possível inicializar o DFPlayer Mini."));
     Serial.println(F("Por favor, verifique:"));
     Serial.println(F(" - Fiação (RX, TX, VCC, GND)"));
     Serial.println(F(" - Cartão SD inserido e formatado corretamente"));
     Serial.println(F(" - Arquivos na pasta /mp3/ com nomes 0001.mp3, etc."));
    // while (true); // Trava a execução se houver erro
   }

  Serial.println(F("DFPlayer Mini inicializado com sucesso!"));
  Serial.println(F("-----------------------------------------"));
  
   myDFPlayer.volume(30);  // Define o volume (0 a 30)
   myDFPlayer.play(1);     // Toca a primeira música para confirmar que está funcionando
  
  delay(3000); // Espera um pouco

  Serial.println(F("Setup concluído. O módulo está ocioso (IDLE)."));
  Serial.println(F(">>> Você pode medir a corrente do modo OCIOSO agora. <<<"));
  Serial.println(F("-----------------------------------------"));
  Serial.println(F("Envie comandos pelo Monitor Serial:"));
  Serial.println(F(" 'p' -> para tocar a música 1 (Play)"));
  Serial.println(F(" 's' -> para colocar o módulo para dormir (Sleep)"));
  Serial.println(F(" 'w' -> para acordar o módulo (Wake/Start)"));
  Serial.println(F("-----------------------------------------"));
}

void loop() {
  // Verifica se um comando foi enviado pelo Monitor Serial
  if (Serial.available()) {
    char command = Serial.read();

    switch (command) {
      case 'p':
        Serial.println(F("Comando recebido: Play (tocando a faixa 1)"));
        Serial.println(F(">>> Você pode medir a corrente do modo PLAY agora. <<<"));
        myDFPlayer.play(1);
        break;

      case 's':
        Serial.println(F("Comando recebido: Sleep (colocando para dormir)"));
        myDFPlayer.sleep();
        Serial.println(F(">>> O módulo está dormindo. Meça a corrente do modo SLEEP agora. <<<"));
        break;

      case 'w':
        Serial.println(F("Comando recebido: Wake (acordando com o comando reset)"));
        myDFPlayer.reset();
        mySoftwareSerial.begin(9600);
        delay(1000); // Pequena pausa para garantir que o módulo acordou
        Serial.println(F(">>> Módulo acordado e OCIOSO. Meça a corrente do modo OCIOSO novamente. <<<"));
        Serial.println(F("Envie 'p' para confirmar que ele toca um som."));
        break;
    }
  }
}