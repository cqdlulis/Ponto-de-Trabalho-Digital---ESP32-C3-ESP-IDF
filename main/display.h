#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_PORT I2C_NUM_0
#define SDA_PIN 8
#define SCL_PIN 9
#define LCD_ADDR 0x27

static void i2c_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
}

static void lcd_write_byte(uint8_t data) {
    i2c_master_write_to_device(I2C_PORT, LCD_ADDR, &data, 1, pdMS_TO_TICKS(100));
}

static void lcd_pulse(uint8_t data) {
    lcd_write_byte(data | 0x04);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_write_byte(data & ~0x04);
    vTaskDelay(pdMS_TO_TICKS(1));
}

static void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | 0x08 | mode;
    lcd_pulse(data);
}

static void lcd_send(uint8_t value, uint8_t mode) {
    lcd_send_nibble(value & 0xF0, mode);
    lcd_send_nibble((value << 4) & 0xF0, mode);
}

static void lcd_cmd(uint8_t cmd) {
    lcd_send(cmd, 0x00);
}

static void lcd_char(char c) {
    lcd_send(c, 0x01);
}

static void lcd_init(void) {
    vTaskDelay(pdMS_TO_TICKS(50));

    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_nibble(0x20, 0);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);

    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_clear(void) {
    lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_set_cursor(uint8_t col, uint8_t row) {
    uint8_t offsets[] = {0x00, 0x40};
    lcd_cmd(0x80 | (col + offsets[row]));
}

static void lcd_print(const char *str) {
    while (*str) {
        lcd_char(*str++);
    }
}

#endif