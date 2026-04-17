#include <stdio.h>
#include "ssd1306.h"

uint8_t buffer[buffer_size] = {0}; /// Buffer to hold the display data, initialized to 0

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

void ssd1306_clear(){
    for(int i=0; i<buffer_size; i++){
        buffer[i] = 0x00; /// Clear the buffer
    }
}

void ssd1306_draw_pixel(int x, int y){
    int page = y / 8;                   /// Each page is 8 pixels high
    int index = page * 128 + x;         /// Calculate the index in the buffer
    buffer[index] |= (1 << (y % 8));    /// Set the corresponding bit for the pixel
}

void ssd1306_update(i2c_master_dev_handle_t __dev_handle) {
    for (int page = 0; page < 8; page++) {
        send_command(__dev_handle, 0xB0 + page);
        send_command(__dev_handle, 0x00);
        send_command(__dev_handle, 0x10);

        send_data(__dev_handle, &buffer[page * 128], 128);
    }
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

    send_command(dev_handle, 0xB0);       /// Set page start address for page addressing mode
    send_command(dev_handle, 0xC8);       /// COM scan direction (flip vertically)
    send_command(dev_handle, 0x00);       /// Set lower column address
    send_command(dev_handle, 0x10);       /// Set higher column address

    send_command(dev_handle, 0x40);       /// Set start line address
    send_command(dev_handle, 0x81);       /// Set contrast control
    send_command(dev_handle, 0xFF);       /// Max contrast

    send_command(dev_handle, 0xA1);       /// Segment re-map (flip horizontally)
    send_command(dev_handle, 0xA6);       /// Normal display (not inverted)
    send_command(dev_handle, 0xA8);       /// Set multiplex ratio
    send_command(dev_handle, 0x3F);       /// 1/64 duty

    send_command(dev_handle, 0xA4);       /// Output RAM to display
    send_command(dev_handle, 0xD3);       /// Set display offset
    send_command(dev_handle, 0x00);       /// No offset

    send_command(dev_handle, 0xD5);       /// Set display clock divide ratio
    send_command(dev_handle, 0xF0);       /// Set divide ratio

    send_command(dev_handle, 0xD9);       /// Set pre-charge period
    send_command(dev_handle, 0x22);       /// Set pre-charge period (0

    send_command(dev_handle, 0xDA);       /// Set COM pins hardware configuration
    send_command(dev_handle, 0x12);       /// for 128x64 display

    send_command(dev_handle, 0xDB);       /// Set VCOMH deselect level
    send_command(dev_handle, 0x20);       /// Set VCOMH deselect

    send_command(dev_handle, 0x8D);       /// Set charge pump
    send_command(dev_handle, 0x14);       /// Enable charge pump

    send_command(dev_handle, 0xAF);       /// Display on


    ssd1306_clear(); /// Clear the display buffer
    for(int i=0; i<64; i++){
        ssd1306_draw_pixel(i,i); /// Draw a pixel at (10, 10)
    }
    ssd1306_update(dev_handle);
}