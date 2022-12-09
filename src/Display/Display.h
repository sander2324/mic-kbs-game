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


class DisplayClass {
    public:
        DisplayClass();
        void begin();
        void show_square();

    private:
        void startup();
        void init_display_registers();
        void spi_init();
        inline void spi_begin();
        inline void spi_end();
        uint8_t spi_transfer(uint8_t data);
        void send_command(uint8_t command);
        void send_command(uint8_t command, uint8_t* args, uint16_t args_len);
};


extern DisplayClass Display;

#endif
