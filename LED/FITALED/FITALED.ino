#include <FastLED.h>

#define LED_PIN     15  // Pino de dados onde a fita está conectada
#define NUM_LEDS    15  // Quantidade de LEDs na sua fita. Mude se for diferente!

// Array que armazena o estado de cada LED
CRGB leds[NUM_LEDS];

void setup() {
    // Inicia a comunicação serial para possíveis mensagens de debug
    Serial.begin(115200);
    Serial.println("Iniciando o teste da fita de LED...");

    // Configura a biblioteca FastLED com o tipo de fita, pino e ordem de cores
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    
    // Define o brilho global (0 a 255). 128 é um bom valor para não forçar a fonte de energia.
    FastLED.setBrightness(128); 
    
    // Garante que a fita comece apagada
    FastLED.clear();
    FastLED.show();
}

void loop() {
    // --- Teste 1: Cores Sólidas ---
    // Pinta todos os LEDs de Vermelho
    Serial.println("Teste: Vermelho");
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show(); // Envia os dados para a fita de LED
    delay(1000);    // Mantém a cor por 1 segundo

    // Pinta todos os LEDs de Verde
    Serial.println("Teste: Verde");
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();
    delay(1000);

    // Pinta todos os LEDs de Azul
    Serial.println("Teste: Azul");
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    FastLED.show();
    delay(1000);


    // --- Teste 2: Efeito Cometa (Branco) ---
    Serial.println("Teste: Efeito Cometa");
    for(int i = 0; i < NUM_LEDS; i++) {
        // Apaga todos os LEDs
        FastLED.clear();
        
        // Acende apenas o LED da posição 'i'
        leds[i] = CRGB::White;
        
        // Mostra a mudança
        FastLED.show();
        
        // Pequena pausa para o efeito ser visível
        delay(50);
    }

    // Apaga a fita antes de reiniciar o loop
    FastLED.clear();
    FastLED.show();
    delay(1000);
}