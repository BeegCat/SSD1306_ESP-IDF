#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SDA 21
#define SCL 22

#define I2C_ADDRESS 0x3C
#define I2C_FREQUENCY 100000    // standard mode frequency; can be pushed to 400kHz for fast mode

#define buffer_size (128 * 64) / 8


void send_command(i2c_master_dev_handle_t __dev_handle, uint8_t command);
void send_data(i2c_master_dev_handle_t __dev_handle, uint8_t *data, int len);
void ssd1306_clear();
void ssd1306_draw_pixel(int x, int y);

#endif // SSD1306_H