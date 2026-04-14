#include <stdio.h>
#include "ssd1306.h"

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

     
}


/**
 * i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = TEST_I2C_PORT,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
};

i2c_master_bus_handle_t bus_handle;
ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x58,
    .scl_speed_hz = 100000,
};

i2c_master_dev_handle_t dev_handle;
ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
 */