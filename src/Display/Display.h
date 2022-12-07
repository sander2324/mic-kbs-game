#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <avr/io.h>

#define DISPLAY_NOOP_COMMAND 0x00
#define DISPLAY_RESET_COMMAND 0x01
#define DISPLAY_COLUMN_ADDRESS_SET_COMMAND 0x2A
#define DISPLAY_PAGE_ADDRESS_SET_COMMAND 0x2B
#define DISPLAY_MEMORY_WRITE_COMMAND 0x2C


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
        template <typename T> void send_command(uint8_t command, T* args, uint8_t args_len);
};


extern DisplayClass Display;

#endif
