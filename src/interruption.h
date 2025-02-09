#ifndef _INTERRUPTION_H
#define _INTERRUPTION_H

#include "pico/stdlib.h"

typedef void (*buttons_irq_handler_t)(uint gpio, uint32_t events);

void initialize_buttons(uint button_a_pin, uint button_b_pin, buttons_irq_handler_t handler);
#endif