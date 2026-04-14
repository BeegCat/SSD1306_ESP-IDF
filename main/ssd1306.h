#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SDA 21
#define SCL 22

#define I2C_ADDRESS 0x3C
#define I2C_FREQUENCY 100000    // standard mode frequency; can be pushed to 400kHz for fast mode

#define buffer (128 * 64) / 8

#endif // SSD1306_H