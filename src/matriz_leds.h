#ifndef _MATRIZ_LEDS_H
#define _MATRIZ_LEDS_H
#include "hardware/pio.h"

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
} RGB_color;

// Definição de tipo da matriz de leds
typedef RGB_color Led_matrix_layout[5][5]; 

uint32_t generate_color_binary(double red, double green, double blue);

uint initialize_matrix(PIO pio,uint led_matrix_pin);

void draw_on_matrix(Led_matrix_layout layout, PIO pio, uint sm);

RGB_color get_color_by_RGB_code(int red, int green, int blue);

Led_matrix_layout* get_number_layout(int number);

#endif