#include <SPI.h>
#include <MFRC522.h>
#include <FastLED.h>

// Configurações dos pinos para os 4 leitores
#define SS_PIN_1 33
#define RST_PIN_1 32
#define SS_PIN_2 26
#define RST_PIN_2 25
#define SS_PIN_3 4
#define RST_PIN_3 27 // Corrigi o pino RST_PIN_3
#define SS_PIN_4 17
#define RST_PIN_4 16 // Corrigi o pino RST_PIN_4

MFRC522 rfidReaders[4] = {
  MFRC522(SS_PIN_1, RST_PIN_1),
  MFRC522(SS_PIN_2, RST_PIN_2),
  MFRC522(SS_PIN_3, RST_PIN_3),
  MFRC522(SS_PIN_4, RST_PIN_4)
};

// Array para armazenar os últimos UIDs lidos de cada leitor
byte nuidPICC[4][4];

// Configurações dos pinos dos LEDs
const int ledPin1 = 12; // D12
const int ledPin2 = 14; // D14
const int ledPin3 = 15; // D15
const int ledPin4 = 2;  // D2
const int NUM_LEDS_POR_FITA = 24;
const int NUM_FITAS = 4;
const int NUM_LEDS = NUM_LEDS_POR_FITA * NUM_FITAS;

// Define um array de CRGB para controlar as cores dos LEDs
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  SPI.begin();

  // Inicializa os leitores RFID
  for (int i = 0; i < 4; i++) {
    rfidReaders[i].PCD_Init();
    Serial.print("Leitor ");
    Serial.print(i + 1);
    Serial.println(" inicializado");
  }
  Serial.println(F("Sistema pronto para ler tags em 4 leitores"));

  // Inicializa a biblioteca FastLED para cada fita de LED
  FastLED.addLeds<WS2812, ledPin1, GRB>(leds, NUM_LEDS_POR_FITA);
  FastLED.addLeds<WS2812, ledPin2, GRB>(leds + NUM_LEDS_POR_FITA, NUM_LEDS_POR_FITA);
  FastLED.addLeds<WS2812, ledPin3, GRB>(leds + (2 * NUM_LEDS_POR_FITA), NUM_LEDS_POR_FITA);
  FastLED.addLeds<WS2812, ledPin4, GRB>(leds + (3 * NUM_LEDS_POR_FITA), NUM_LEDS_POR_FITA);

  // Apaga todos os LEDs inicialmente
  FastLED.clear();
  FastLED.show();
}

void loop() {
  for (int i = 0; i < 4; i++) {
    MFRC522& leitor = rfidReaders[i];

    // Verifica novos cartões apenas no leitor atual
    if (!leitor.PICC_IsNewCardPresent()) continue;

    // Tenta ler o serial do cartão
    if (!leitor.PICC_ReadCardSerial()) continue;

    Serial.print(F("Leitor "));
    Serial.print(i + 1);
    Serial.println(F(" detectou:"));

    // Verifica o tipo do cartão
    MFRC522::PICC_Type piccType = leitor.PICC_GetType(leitor.uid.sak);

    // Valida se é MIFARE Classic
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Tag não é MIFARE Classic"));
      continue;
    }

    // Verifica se é um novo UID
    bool novoCartao = false;
    for (byte j = 0; j < 4; j++) {
      if (leitor.uid.uidByte[j] != nuidPICC[i][j]) {
        novoCartao = true;
        break;
      }
    }

    if (novoCartao) {
      Serial.println(F("Novo cartão detectado!"));

      // Armazena novo UID
      memcpy(nuidPICC[i], leitor.uid.uidByte, 4);

      // Exibe dados
      Serial.print(F("UID (hex):"));
      printHex(leitor.uid.uidByte, leitor.uid.size);
      Serial.println();
      Serial.print(F("UID (dec):"));
      printDec(leitor.uid.uidByte, leitor.uid.size);
      Serial.println();

      // Acende a fita de LED correspondente ao leitor que detectou o cartão
      CRGB color;
      switch (i) {
        case 0: color = CRGB::Green; break;  // Leitor 1 -> LED D12
        case 1: color = CRGB::Blue; break;   // Leitor 2 -> LED D14
        case 2: color = CRGB::Red; break;    // Leitor 3 -> LED D15
        case 3: color = CRGB::Yellow; break; // Leitor 4 -> LED D2
        default: color = CRGB::White; break;
      }

      for (int j = 0; j < NUM_LEDS_POR_FITA; j++) {
        leds[i * NUM_LEDS_POR_FITA + j] = color;
      }
      FastLED.show();
      delay(500); // Mantém os LEDs acesos por 500ms

      // Apaga a fita de LED
      for (int j = 0; j < NUM_LEDS_POR_FITA; j++) {
        leds[i * NUM_LEDS_POR_FITA + j] = CRGB::Black;
      }
      FastLED.show();
    } else {
      Serial.println(F("Cartão já cadastrado"));
    }

    // Finaliza operação
    leitor.PICC_HaltA();
    leitor.PCD_StopCrypto1();
  }
}

// Funções auxiliares (mantidas do código original)
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}