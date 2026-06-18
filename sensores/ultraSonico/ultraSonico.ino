#include <Wire.h> // Necessário para comunicação I2C
#include <Adafruit_GFX.h> // Biblioteca gráfica da Adafruit
#include <Adafruit_SSD1306.h> // Biblioteca específica para o display SSD1306

// Define os pinos do sensor ultrassônico
const int pinoTrig = 5;  // Pino Trig conectado ao GPIO2 do ESP32
const int pinoEcho = 18;  // Pino Echo (depois do divisor de tensão) conectado ao GPIO4 do ESP32

// Define a velocidade do som em cm/microssegundo (0.0343 cm/µs)
const float velocidadeDoSom = 0.0343;

// Configuração do display OLED
// Defina a largura e altura do seu display
#define SCREEN_WIDTH 128 // Largura do display OLED (em pixels)
#define SCREEN_HEIGHT 64 // Altura do display OLED (em pixels)

// Defina o endereço I2C do seu display (geralmente 0x3C ou 0x3D)
#define OLED_RESET -1 // Pino de reset (use -1 se estiver usando um pino de reset compartilhado ou nenhum pino de reset)

// Cria o objeto do display SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Inicializa a comunicação serial (ainda útil para depuração)
  Serial.begin(115200);

  // Define os pinos do sensor como entrada ou saída
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  // Inicializa o display OLED
  // Verifica se o display foi inicializado corretamente
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // O endereço 0x3C é o mais comum, mas pode ser 0x3D
    Serial.println(F("Falha na inicializacao do SSD1306"));
    for (;;); // Loop infinito se o display não inicializar
  }

  // Limpa o buffer do display
  display.clearDisplay();

  // Configura a cor do texto (WHITE ou BLACK, dependendo do display)
  display.setTextColor(SSD1306_WHITE);

  // Exibe uma mensagem inicial no display
  display.setTextSize(1); // Tamanho do texto
  display.setCursor(0, 0); // Posição inicial do cursor (coluna, linha)
  display.println("Iniciando sensor...");
  display.display(); // Mostra o conteúdo do buffer no display
  delay(2000); // Espera 2 segundos
}

void loop() {
  // Limpa o pino Trig
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);

  // Envia um pulso de 10 microssegundos
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  // Mede a duração do pulso
  long duracao = pulseIn(pinoEcho, HIGH);

  // Calcula a distância
  float distanciaCm = (duracao * velocidadeDoSom) / 2;

  // Limpa o display para a nova leitura
  display.clearDisplay();

  // Configura o texto para exibir a distância
  display.setTextSize(2); // Um tamanho de texto maior para a distância
  display.setCursor(0, 0); // Inicia no canto superior esquerdo
  display.print("Dist: ");
  display.print(distanciaCm);
  display.println(" cm");

  // Exibe a informação no display
  display.display();

  // Pequeno atraso para não atualizar o display muito rápido
  delay(500);
}