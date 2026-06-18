# ⚡ ESP32 Playground

[![Status](https://img.shields.io/badge/Status-Em%20Desenvolvimento-green)](#)
[![Plataforma](https://img.shields.io/badge/Plataforma-ESP32-blue)](#)
[![Linguagem](https://img.shields.io/badge/Linguagem-C%2F%2B%2B-orange)](#)

Repositório dedicado ao desenvolvimento, teste e documentação de códigos para o ecossistema **ESP32**. Este espaço serve como um laboratório prático para explorar integrações de hardware, controle de atuadores, leitura de sensores e otimização de sistemas embarcados para projetos de robótica e automação.

## 🗂️ Estrutura do Repositório

Os experimentos estão organizados de forma modular, focando em diferentes aspetos da eletrônica e programação de baixo nível:

* **`/sensores`**
    * **Ultrassónico (HC-SR04):** Medição de distância com feedback em displays OLED (SSD1306) via I2C.
    * **Temperatura e Humidade:** Integração com sensores DHT22.
    * **Laser ToF (VL53L0X):** Medição de precisão e testes de gestão de energia (Hardware Standby/XSHUT).
    * **Magnéticos:** Testes de leitura com múltiplos sensores de Efeito Hall.

* **`/motores`**
    * **Motores DC N20:** Controle de direção, aplicação de PWM (Ponte H) e leitura de Encoders com cálculo de pulsos.
    * **Servos:** Manipulação de ângulos (0-180°) e servos de rotação contínua (360°).
    * **Motores de Passo:** Implementação do método *half-step* e manipulação direta das bobinas.

* **`/RFID`**
    * Integração com módulos MFRC522 (SPI) para leitura de tags MIFARE.
    * Mapeamento de UIDs e implementação de modos de baixo consumo (*Soft Power Down*).

* **`/LED`**
    * Controle de fitas LED endereçáveis (WS2812B) utilizando a biblioteca FastLED para criação de animações e feedback visual.

* **`/curiosidades-esp32`**
    * **Deep Sleep:** Testes de gestão extrema de bateria e *wakeup* por pinos externos.
    * **Scanner I2C:** Utilitário para mapeamento de endereços de periféricos conectados ao barramento I2C.
    * **Áudio:** Controle de reprodução MP3 via comunicação Serial com o módulo DFPlayer Mini.

## 🛠️ Tecnologias e Protocolos

* **Microcontrolador:** ESP32
* **Linguagem:** C/C++
* **Comunicação:** I2C, SPI, PWM, UART (Serial)
* **Controlo de Versões:** Git & GitHub

## 🚀 Como Utilizar

1. Clone este repositório para a sua máquina local:
   ```bash
   git clone https://github.com/jhowsbDiem/esp32-playground.git

2. Navegue até ao módulo ou projeto de interesse.

3. Consulte os comentários no código-fonte (.ino ou .cpp) para obter detalhes específicos sobre os esquemas de ligação dos pinos (pinout) e as bibliotecas necessárias.

👨‍💻 Autor
Jonathan Dos Santos Bezerra
Assistente Técnico de Eletrónica | Desenvolvedor de Sistemas Embarcados