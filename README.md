# Projeto BitDogLab: Comunicação Serial, Interrupções e Display SSD1306 com RP2040

Este projeto demonstra o controle integrado de uma placa BitDogLab utilizando o microcontrolador RP2040. Nele, foram implementadas as seguintes funcionalidades:

- *Comunicação Serial via USB:*  
  Permite o envio e recebimento de caracteres através do Serial Monitor (VS Code). Cada caractere recebido é exibido no display SSD1306.  
  - Se o caractere for um dígito (0-9), um símbolo correspondente é desenhado em uma matriz 5x5 de LEDs endereçáveis WS2812.

- *Controle de LEDs RGB e Matriz WS2812:*  
  - *Botão A (GPIO 5):* Alterna o estado do LED RGB *verde* (ligado/desligado).  
  - *Botão B (GPIO 6):* Alterna o estado do LED RGB *azul* (ligado/desligado).  
  - Ambos os botões utilizam *interrupções* e *debouncing* (300 ms) para garantir uma operação confiável.  
  - O estado dos LEDs RGB é exibido tanto no display quanto enviado via UART (Serial Monitor).

- *Display SSD1306 (128x64) via I2C:*  
  - Exibe o último caractere recebido pelo Serial Monitor e o estado atual dos LEDs (verde e azul).  
  - O display é configurado e atualizado periodicamente com as informações do sistema.

## Componentes Utilizados

- *Placa BitDogLab* com microcontrolador *RP2040*.
- *Matriz 5x5 de LEDs WS2812* – conectada à GPIO 7.
- *LED RGB* – com os pinos conectados às GPIOs 11 (verde), 12 (azul) e 13 (vermelho).
- *Botões:*  
  - Botão A: conectado à GPIO 5.  
  - Botão B: conectado à GPIO 6.
- *Display SSD1306* (128x64) – conectado via I2C com SDA na GPIO 14 e SCL na GPIO 15.

## Funcionalidades do Projeto

1. **Modificação da Biblioteca font.h:**  
   - Inclusão de caracteres minúsculos. (Utilize sua criatividade para definir os novos caracteres!)

2. *Entrada de Caracteres via PC:*  
   - Utilização do Serial Monitor do VS Code para enviar caracteres.
   - Exibição do caractere recebido no display SSD1306.
   - Se o caractere for um número (0 a 9), o símbolo correspondente é exibido na matriz WS2812.

3. *Interação com Botões (usando Interrupções e Debouncing):*  
   - *Botão A:* Alterna o LED RGB verde, com feedback no display e via Serial Monitor.
   - *Botão B:* Alterna o LED RGB azul, com feedback no display e via Serial Monitor.

## Organização do Código

- *main.c:*  
  Contém a integração de todas as funcionalidades, com a configuração dos módulos de comunicação serial, I2C, controle da matriz de LEDs, gerenciamento dos botões com interrupções e atualização do display.

- *Outros Arquivos e Bibliotecas:*  
  - ssd1306.h e suas implementações: para o controle do display SSD1306.  
  - matriz_leds.h: funções para controle da matriz de LEDs WS2812.  
  - interruption.h: configurações para os botões com tratamento de debouncing.

## Como Compilar e Executar

1. *Configuração do Ambiente:*  
   - Configure o SDK do Raspberry Pi Pico (RP2040) conforme a [documentação oficial](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).
   - Certifique-se de que as bibliotecas utilizadas (SSD1306, WS2812, interrupções) estão corretamente incluídas no seu projeto.

2. *Compilação:*  
   - Utilize o CMake (ou a ferramenta de build que preferir) para compilar o código-fonte.

3. *Upload para a Placa:*  
   - Carregue o firmware compilado na placa BitDogLab.

## Demonstração

Assista ao vídeo de demonstração onde explico o funcionamento completo do projeto e mostro a placa em ação:

[![Demonstração do Projeto](https://img.youtube.com/vi/a1aB4HMHjSg/0.jpg)](https://youtube.com/shorts/a1aB4HMHjSg?si=8r8VbqLGvBdw-wVk)

## Conclusão

Este projeto integra diversos conceitos fundamentais, como comunicação serial, controle de hardware via I2C, uso de interrupções e debouncing, além da manipulação de LEDs comuns e endereçáveis. É uma excelente oportunidade para consolidar o conhecimento em desenvolvimento de sistemas embarcados com o RP2040.