#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ROWS 4
#define COLS 4

// Ajuste os GPIOs conforme seu circuito
int row_pins[ROWS] = {0, 1, 2, 3};
int col_pins[COLS] = {4, 5, 6, 7};

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init() {
    for (int i = 0; i < ROWS; i++) {
        gpio_reset_pin(row_pins[i]);
        gpio_set_direction(row_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(row_pins[i], 1);
    }

    for (int i = 0; i < COLS; i++) {
        gpio_reset_pin(col_pins[i]);
        gpio_set_direction(col_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(col_pins[i], GPIO_PULLUP_ONLY);
    }
}

char keypad_read() {
    for (int row = 0; row < ROWS; row++) {
        // deixa todas as linhas em HIGH
        for (int i = 0; i < ROWS; i++) {
            gpio_set_level(row_pins[i], 1);
        }

        // ativa a linha atual
        gpio_set_level(row_pins[row], 0);

        vTaskDelay(pdMS_TO_TICKS(5));

        for (int col = 0; col < COLS; col++) {
            if (gpio_get_level(col_pins[col]) == 0) {
                vTaskDelay(pdMS_TO_TICKS(50)); 

                while (gpio_get_level(col_pins[col]) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                return keys[row][col];
            }
        }
    }

    return '\0';
}

#endif