#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <avr/io.h>

#define DISPLAY_NOOP_COMMAND 0x00
#define DISPLAY_RESET_COMMAND 0x01
#define DISPLAY_SLEEP_OUT_COMMAND 0x11
#define DISPLAY_GAMMA_SET_COMMAND 0x26
#define DISPLAY_ON_COMMAND 0x29
#define DISPLAY_COLUMN_ADDRESS_SET_COMMAND 0x2A
#define DISPLAY_PAGE_ADDRESS_SET_COMMAND 0x2B
#define DISPLAY_MEMORY_WRITE_COMMAND 0x2C
#define DISPLAY_COLOR_SET_COMMAND 0x2D
#define DISPLAY_INVERSION_OFF_COMMAND 0x20
#define DISPLAY_INVERSION_ON_COMMAND 0x21
#define DISPLAY_MEMORY_ACCESS_CONTROL_COMMAND 0x36
#define DISPLAY_VERTICAL_SCROLLING_START_ADDRESS_COMMAND 0x37
#define DISPLAY_COLMOD_PIXEL_FORMAT_SET_COMMAND 0x3A
#define DISPLAY_FRAME_RATE_CONTROL_NORMAL_MODE_COMMAND 0xB1
#define DISPLAY_FUNCTION_CONTROL_COMMAND 0xB6
#define DISPLAY_POWER_CONTROL_1_COMMAND 0xC0
#define DISPLAY_POWER_CONTROL_2_COMMAND 0xC1
#define DISPLAY_VCOM_CONTROL_1_COMMAND 0xC5
#define DISPLAY_VCOM_CONTROL_2_COMMAND 0xC7
#define DISPLAY_POWER_CONTROL_A_COMMAND 0xCB
#define DISPLAY_POWER_CONTROL_B_COMMAND 0xCF
#define DISPLAY_POSITIVE_GAMMA_CORRECTION_COMMAND 0xE0
#define DISPLAY_NEGATIVE_GAMMA_CORRECTION_COMMAND 0xE1
#define DISPLAY_UNDOCUMENTED_COMMAND_1 0xEF
#define DISPLAY_DRIVER_TIMING_CONTROL_A_COMMAND 0xE8
#define DISPLAY_DRIVER_TIMING_CONTROL_B_COMMAND 0xEA
#define DISPLAY_POWER_ON_SEQUENCE_CONTROL_COMMAND 0xED
#define DISPLAY_ENABLE_3G_COMMAND 0xF2
#define DISPLAY_PUMP_RATIO_CONTROL_COMMAND 0xF7


#define DISPLAY_ADDRESS_PARAM_SIZE 4
#define DISPLAY_COLUMN_PIXEL_AMOUNT 240
#define DISPLAY_ROW_PIXEL_AMOUNT 320


class DisplayClass {
    public:
        DisplayClass();
        void begin();
        void fill_screen(uint16_t color);
        void draw_pixel(uint16_t y, uint16_t row, uint16_t color);
        void draw_rectangle(
            uint16_t y_start,
            uint16_t x_start,
            uint16_t y_end,
            uint16_t x_end,
            uint16_t color
        );
        void draw_circle(
            uint16_t y,
            uint16_t x,
            uint16_t radius,
            uint16_t color
        );
        void invert_colors();

    private:
        void start_display_startup_sequence();
        inline void init_registers();
        void spi_init();
        inline void spi_begin();
        inline void spi_end();
        uint8_t spi_transfer(uint8_t data);
        inline void transfer_pixel_color_spi(uint16_t color);
        void send_command(uint8_t command, bool end_spi_after_command = true);
        void send_command(uint8_t command, uint8_t* args, uint32_t args_len);
        void set_address_window(uint16_t y_start, uint16_t x_start, uint16_t y_end, uint16_t x_end);
        bool is_inverted;
};


extern DisplayClass Display;

#endif
