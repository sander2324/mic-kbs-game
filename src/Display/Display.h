#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <avr/io.h>

#define DISPLAY_NOOP_COMMAND 0x00
#define DISPLAY_RESET_COMMAND 0x01
#define DISPLAY_COLUMN_ADDRESS_SET_COMMAND 0x2A
#define DISPLAY_PAGE_ADDRESS_SET_COMMAND 0x2B
#define DISPLAY_MEMORY_WRITE_COMMAND 0x2C
#define DISPLAY_COLOR_SET_COMMAND 0x2D

#define DISPLAY_ADDRESS_PARAM_SIZE 4
#define DISPLAY_COLUMN_PIXEL_AMOUNT 240
#define DISPLAY_ROW_PIXEL_AMOUNT 320


class DisplayClass {
    public:
        DisplayClass();
        void begin();
        void fill_screen(uint16_t color);
        void fill_rect(
            uint16_t column_start,
            uint16_t row_start,
            uint16_t column_end,
            uint16_t row_end,
            uint16_t color
        );

    private:
        void startup();
        void init_display_registers();
        void spi_init();
        inline void spi_begin();
        inline void spi_end();
        uint8_t spi_transfer(uint8_t data);
        inline void transfer_pixel_spi(uint16_t x, uint16_t y, uint16_t color);
        void send_command(uint8_t command, bool end_spi_after_command = true);
        void send_command(uint8_t command, uint8_t* args, uint32_t args_len);
        void set_address_window(uint16_t column_start, uint16_t row_start, uint16_t column_end, uint16_t row_end);
};


extern DisplayClass Display;

#endif
