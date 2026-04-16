#include <stdio.h>
#include "ssd1306.h"

void send_command(i2c_master_dev_handle_t __dev_handle, uint8_t command) {
    uint8_t data[2] = {0x00, command};              /// 0x00 indicates a command
    i2c_master_transmit(__dev_handle, data, 2, -1); /// -1 for no timeout
}

void send_data(i2c_master_dev_handle_t __dev_handle, uint8_t *data, int len) {
    uint8_t temp[129];                              /// 1 byte for control + 128 bytes for data
    temp[0] = 0x40;                                 /// 0x40 indicates data

    for(int i = 0; i < len; i++){
        temp[i+1] = data[i];
    }
    
    i2c_master_transmit(__dev_handle, temp, len + 1, -1); 
}

void app_main(void)
{

    /**
     * @brief I2C configuration for SSD1306 display
     * @note We have decided to use the new I2C driver (driver/i2c_master.h)
     * initially we used the old driver (driver/i2c.h) but it is now deprecated and
     * we want to migrate to the new driver to follow the IDF's latest version
     * also to ensure compatibility with future i2c updates.
     * 
     * */

    i2c_master_bus_config_t i2c_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = SCL,
        .sda_io_num = SDA,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    i2c_new_master_bus(&i2c_config, &bus_handle);

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_ADDRESS,
        .scl_speed_hz = I2C_FREQUENCY,
    };

    i2c_master_dev_handle_t dev_handle;

    i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);

    /**
     * @brief 
     * 
     * 
     */

    vTaskDelay(100 / portTICK_PERIOD_MS); /// Delay to ensure the display is ready

    send_command(dev_handle, 0xAE);       /// Display off
    send_command(dev_handle, 0x20);       /// Set memory addressing mode
    send_command(dev_handle, 0x10);       /// Set page addressing mode
}