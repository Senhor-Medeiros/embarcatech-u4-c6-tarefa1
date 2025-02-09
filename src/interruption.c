#include <stdio.h>
#include "interruption.h"
#include "pico/stdlib.h"

// Configura e inicializa a GPIO dos botões, além de configurar as interruções para os botões
void initialize_buttons(uint button_a_pin, uint button_b_pin, buttons_irq_handler_t handler){
    // Configura os botões como entradas em pull-up
    gpio_init(button_a_pin);
    gpio_init(button_b_pin);
    gpio_set_dir(button_a_pin, GPIO_IN);
    gpio_set_dir(button_b_pin, GPIO_IN);
    gpio_pull_up(button_a_pin);
    gpio_pull_up(button_b_pin);

    // Configura a interrupção para os botões
    gpio_set_irq_enabled_with_callback(button_a_pin, GPIO_IRQ_EDGE_FALL, true, handler);
    gpio_set_irq_enabled_with_callback(button_b_pin, GPIO_IRQ_EDGE_FALL, true, handler);
}