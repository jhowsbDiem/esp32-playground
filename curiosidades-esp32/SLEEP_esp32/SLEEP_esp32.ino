

/**
 * @brief Função de teste para colocar o ESP32 em Deep Sleep.
 * O dispositivo acordará quando o pino 33 (pinoReset) for colocado em nível BAIXO.
 */
void entrarEmSleepTeste() {
  Serial.println("TESTE: Entrando em Deep Sleep em 5 segundos...");
  

  delay(1000); // Pausa para garantir que a mensagem foi vista


  delay(6000); // Tempo total de 7s

  Serial.println("Desativando interrupcao de reset para dormir...");
  

  Serial.println("Configurando pino 33 (pinoReset) como fonte de wakeup...");
  
  // Configura o pino 33 para acordar o ESP32 quando estiver em nível ALTO (1).
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); 

  Serial.println("Ate logo! Entrando em deep sleep agora.");
  

  
  esp_deep_sleep_start();
}

void setup() {
  
  Serial.begin(115200);
  
  entrarEmSleepTeste();

}

void loop() {
  // put your main code here, to run repeatedly:

}
