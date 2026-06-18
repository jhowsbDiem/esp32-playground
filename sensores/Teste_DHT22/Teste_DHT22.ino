// Inclui as bibliotecas necessárias
#include "DHT.h"

// --- Configurações ---
// Define o pino ao qual o sensor DHT está conectado
#define DHTPIN 4       // Estou usando o GPIO 4 (D4)

// Define o tipo de sensor DHT que estamos usando (DHT22 ou AM2302)
#define DHTTYPE DHT22

// Inicializa o objeto do sensor DHT com os pinos e o tipo definidos
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inicia a comunicação serial a uma velocidade de 9600 bauds para depuração
  Serial.begin(115200);
  Serial.println("Teste do sensor DHT22 com ESP32");

  // Inicia o sensor
  dht.begin();
}

void loop() {
  // O sensor DHT é um pouco lento, então esperamos 2 segundos entre as leituras
  delay(2000);

  // Lê a umidade do ar. A leitura leva cerca de 250ms.
  float umidade = dht.readHumidity();
  
  // Lê a temperatura em Celsius (padrão)
  float temperatura_c = dht.readTemperature();
  
  // Lê a temperatura em Fahrenheit (isTemperatureFahrenheit = true)
  // float temperatura_f = dht.readTemperature(true);

  // --- Verificação de Erros ---
  // Verifica se alguma das leituras falhou. isnan() checa se o valor não é um número (Not-a-Number).
  if (isnan(umidade) || isnan(temperatura_c)) {
    Serial.println("Falha ao ler dados do sensor DHT!");
    return; // Sai da função loop() e tenta novamente na próxima iteração
  }

  // --- Exibição dos Dados ---
  // Se as leituras foram bem-sucedidas, imprime os valores no Monitor Serial
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%  |  "); // O caractere '%' é impresso como texto
  Serial.print("Temperatura: ");
  Serial.print(temperatura_c);
  Serial.println(" °C");
}