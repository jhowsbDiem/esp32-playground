#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configurações do Display OLED ---
#define SCREEN_WIDTH 128 // Largura
#define SCREEN_HEIGHT 64 // Altura
#define OLED_RESET -1 // Pino de reset
#define SCREEN_ADDRESS 0x3C // Endereço I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Configurações dos Sensores ---
#define SENSOR_POWER_PIN 2 // D2 no ESP32 é o GPIO 2

// Pinos de DADOS dos 5 sensores Hall
const int sensorPins[] = {4, 5, 16, 17, 18};
const int numSensors = 5; // Quantidade de sensores

void setup() {
  Serial.begin(115200);

  // 1. Liga o pino de alimentação dos sensores
  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, HIGH); // Fornece energia

  // 2. Configura os pinos de dados dos sensores como INPUT_PULLUP
  for (int i = 0; i < numSensors; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
  }

  // 3. Inicia o display SSD1306
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao iniciar o display SSD1306!"));
    while(1) delay(10); 
  }

  // Prepara o display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // 1. Limpar o display a cada atualização
  display.clearDisplay();
  display.setCursor(0, 0); // Posiciona no topo
  
  // Define um título centralizado
  display.setCursor(18, 0); // (128 - (15 chars * 6 pixels))/2 = ~18
  display.println(F("--- TESTE HALL ---"));
  display.println(F("")); // Pula uma linha

  // 2. Lê e imprime o estado de cada sensor
  for (int i = 0; i < numSensors; i++) {
    // Lê o pino
    int state = digitalRead(sensorPins[i]);

    // Imprime o nome do pino
    display.print(F("Pino "));
    if (sensorPins[i] < 10) {
      display.print(F(" ")); // Adiciona espaço (Pino  4)
    }
    display.print(sensorPins[i]);
    display.print(F(": "));

    // Imprime o estado com a lógica "0 livre / 1 ativo"
    if (state == HIGH) { // HIGH (1) = Sem imã
      display.println(F("ATIVO (1)")); // Mostra 1
    } else { // LOW (0) = Com imã
      display.println(F("LIVRE (0)")); // Mostra 0 
    }
  }

  // 3. Enviar o frame para o display
  display.display();

  // Pequeno delay
  delay(100); 
}