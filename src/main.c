#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>            // Para isdigit()
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"     // Para uso da matriz WS2812 via PIO
#include "hardware/gpio.h"    // Para controle dos GPIOs
#include "ssd1306.h"          // Biblioteca do display (ajuste o caminho se necessário)
#include "matriz_leds.h"      // Biblioteca para a matriz de LED's WS2812
#include "interruption.h"     // Biblioteca para configuração dos botões com interrupção

// *********************
// *** DEFINIÇÕES  ****
// *********************

// LEDs RGB (pinos da BitDogLab)
#define RED_LED_PIN         13  // GPIO que controla o LED vermelho
#define GREEN_LED_PIN       11  // GPIO que controla o LED verde
#define BLUE_LED_PIN        12  // GPIO que controla o LED azul

// Matriz de LED's (WS2812 – 5x5)
#define LED_MATRIX_PIN      7   // GPIO conectada à matriz de LED's

// Botões
#define BUTTON_A_PIN        5   // Botão A (para controlar o LED verde)
#define BUTTON_B_PIN        6   // Botão B (para controlar o LED azul)

// Debouncing (300 ms em microssegundos)
#define DEBOUNCING_INTERVAL 300000

// Comunicação I2C com o display SSD1306
#define I2C_PORT            i2c1
#define DISPLAY_I2C_SDA_PIN 14  // Pino SDA
#define DISPLAY_I2C_SCL_PIN 15  // Pino SCL
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_CLOCK_FREQUENCY (400 * 1000) // 400 kHz

// Dimensões do display
#define WIDTH 128
#define HEIGHT 64

// Máscaras para inicializar e configurar os pinos do LED RGB
#define LED_RGB_MASK         ((1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN))
#define LED_RGB_GPIO_DIR_MASK ((1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN))

// *********************
// *** VARIÁVEIS GLOBAIS
// *********************

// Variáveis de estado para os LEDs (usadas também na atualização do display)
volatile bool green_led_state = false;
volatile bool blue_led_state = false;
volatile char last_char_input = ' ';  // Armazena o último caractere recebido

// Variáveis globais para o display e a porta PIO (para a matriz)
ssd1306_t display;
PIO pio_used = pio0;  // Usaremos o pio0 para a matriz de LED's

// *********************
// *** FUNÇÕES AUXILIARES
// *********************

/**
 * Função para atualizar o conteúdo do display SSD1306.
 * Exibe:
 *   - O último caractere recebido via Serial;
 *   - O estado atual do LED verde;
 *   - O estado atual do LED azul.
 *
 * Parâmetros:
 *   - display: ponteiro para a estrutura do display.
 *   - color: cor usada para desenhar o retângulo (por exemplo, true para “ligado”);
 *   - fill: se deve limpar o display antes (false = limpa);
 *   - green_led: estado do LED verde (true = on);
 *   - blue_led: estado do LED azul (true = on);
 *   - last_char_inputed: último caractere recebido.
 */
void update_display_data(ssd1306_t* display, bool color, bool fill, bool green_led, bool blue_led, char last_char_inputed) {
    char char_message[50], green_led_message[50], blue_led_message[50];
    
    sprintf(char_message, "Ultimo char: %c", last_char_inputed);
    sprintf(green_led_message, "LED verde: %s", green_led ? "on" : "off");
    sprintf(blue_led_message, "LED azul: %s", blue_led ? "on" : "off");
    
    ssd1306_fill(display, fill);                               // Limpa o display
    ssd1306_rect(display, 2, 2, WIDTH - 4, HEIGHT - 4, color, fill); // Desenha um retângulo de borda
    ssd1306_draw_string(display, char_message, 8, 16);         // Exibe o último caractere
    ssd1306_draw_string(display, green_led_message, 8, 32);      // Exibe o estado do LED verde
    ssd1306_draw_string(display, blue_led_message, 8, 48);       // Exibe o estado do LED azul     
    ssd1306_send_data(display);                                // Atualiza o display
}

/**
 * Função de tratamento de interrupção para os botões.
 * Implementa debouncing e, dependendo do botão, alterna o estado do LED RGB correspondente.
 *
 * Parâmetros:
 *   - gpio: número do pino que disparou a interrupção.
 *   - events: flags dos eventos (não usado neste exemplo).
 */
void buttons_irq_handler(uint gpio, uint32_t events) {
    // Obtém o tempo atual (em microssegundos)
    uint64_t current_time = to_us_since_boot(get_absolute_time());
    static volatile uint64_t last_event_time = 0;

    // Aplica o debouncing (somente processa se 300 ms se passaram)
    if (current_time - last_event_time > DEBOUNCING_INTERVAL) {
        last_event_time = current_time;

        if (gpio == BUTTON_A_PIN) {
            // Alterna o estado do LED verde
            green_led_state = !green_led_state;
            gpio_put(GREEN_LED_PIN, green_led_state);
            printf("Botão A pressionado! LED verde %s\n", green_led_state ? "on" : "off");
        } else if (gpio == BUTTON_B_PIN) {
            // Alterna o estado do LED azul
            blue_led_state = !blue_led_state;
            gpio_put(BLUE_LED_PIN, blue_led_state);
            printf("Botão B pressionado! LED azul %s\n", blue_led_state ? "on" : "off");
        }
    }
}

// *********************
// *** FUNÇÃO PRINCIPAL
// *********************
int main() {
    // Inicializa a comunicação serial via USB
    stdio_init_all();
    
    // Inicializa os pinos dos LEDs RGB (vermelho, verde e azul)
    gpio_init_mask(LED_RGB_MASK);
    gpio_set_dir_masked(LED_RGB_MASK, LED_RGB_GPIO_DIR_MASK);
    
    // Inicializa os botões e configura a interrupção (a função buttons_irq_handler será chamada)
    initialize_buttons(BUTTON_A_PIN, BUTTON_B_PIN, buttons_irq_handler);
    
    // Inicializa a matriz de LED's WS2812 (por exemplo, para exibir números quando dígito for recebido)
    uint sm = initialize_matrix(pio_used, LED_MATRIX_PIN);
    
    // Configura a comunicação I2C para o display SSD1306
    i2c_init(I2C_PORT, DISPLAY_CLOCK_FREQUENCY);
    gpio_set_function(DISPLAY_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_I2C_SDA_PIN);
    gpio_pull_up(DISPLAY_I2C_SCL_PIN);
    
    // Inicializa e configura o display
    ssd1306_init(&display, WIDTH, HEIGHT, false, DISPLAY_I2C_ADDRESS, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_send_data(&display);
    
    // Limpa o display (todos os pixels apagados)
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
    
    // Loop principal
    while (true) {
        // Se a comunicação USB estiver ativa, tenta ler um caractere (não bloqueante)
        if (stdio_usb_connected()) {
            int ch = getchar_timeout_us(0);
            if (ch != PICO_ERROR_TIMEOUT) {
                char character = (char) ch;
                last_char_input = character;  // Atualiza o último caractere recebido
                
                printf("O caractere recebido foi: %c - %d\n", character, (int)character);
                
                // Se o caractere for um dígito ('0' a '9'), atualiza a matriz de LED's com o símbolo correspondente
                if (character >= '0' && character <= '9') {
                    int number = character - '0';
                    draw_on_matrix((*get_number_layout(number)), pio_used, sm);
                }
            }
        }
        
        // Atualiza o display com os dados atuais: último caractere e estado dos LEDs
        update_display_data(&display, true, false, green_led_state, blue_led_state, last_char_input);
        
        sleep_ms(50);
    }
    
    return 0;
}
