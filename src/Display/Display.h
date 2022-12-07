#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <avr/io.h>

#define SPI_SPEED 16000000
#define DISPLAY_RESET_COMMAND 0x01


class DisplayClass {
    public:
        DisplayClass();
        void begin();
        void show_square();

    private:
        void startup();
        void init_display_registers();
        void spi_init();
        uint8_t spi_transfer(uint8_t data);
        void send_command(uint8_t command);
        void send_command(uint8_t command, uint8_t* args, uint8_t args_len);
};


extern DisplayClass Display;

#endif
