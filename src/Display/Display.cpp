#include "Display.h"

#include <util/delay.h>


// Helper functions
#define max(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})


DisplayClass::DisplayClass() {}


void DisplayClass::start_display_startup_sequence() {
    this->send_command(DISPLAY_RESET_COMMAND);
    _delay_ms(150);

    uint8_t startup_commands[] = {
        // Exclusive to the Adafruit ILI
        0xef, 3, 0x03, 0x80, 0x02,
        0xcf, 3, 0x00, 0xC1, 0x30,
        0xed, 4, 0x64, 0x03, 0x12, 0x81,
        0xe8, 3, 0x85, 0x00, 0x78,
        0xcb, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
        0xf7, 1, 0x20,
        0xea, 2, 0x00, 0x00,

        0xc0, 1, 0x23,                                                                                      // Power control VRH[5:0]
        0xc1, 1, 0x10,                                                                                      // Power control SAP[2:0];BT[3:0]
        0xc5, 2, 0x3e, 0x28,                                                                                // VCM control
        0xc7, 1, 0x86,                                                                                      // VCM control2
        0x36, 1, 0x48,                                                                                      // Memory Access Control
        0x37, 1, 0x00,                                                                                      // Vertical scroll zero
        0x3a, 1, 0x55,
        0xb1, 2, 0x00, 0x18,
        0xb6, 3, 0x08, 0x82, 0x27,                                                                          // Display Function Control
        0xf2, 1, 0x00,                                                                                      // 3Gamma Function Disable
        0x26, 1, 0x01,                                                                                      // Gamma curve selected
        0xe0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
        0xe1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
        0x11, 0x80,                                                                                         // Exit Sleep
        0x29, 0x80,                                                                                         // Display on
        0x00                                                                                                // End of list
    };

    uint8_t cmd, x, numArgs;
    uint8_t *addr = startup_commands;
    while ((cmd = *(addr++)) > 0) {
        x = *(addr++);
        numArgs = x & 0x7F;
        this->send_command(cmd, addr, numArgs);
        addr += numArgs;
        if (x & 0x80) {
            _delay_ms(150);
        }
    }
}


void DisplayClass::spi_init() {
    DDRB |= (
        (1 << DDB2) // Set SPI CS (Chip Select) to output mode
        | (1 << DDB3) // Set SPI MOSI (Master Out Slave In) to output
        | (1 << DDB5) // Set SPI SCK (Source Clock) to output
    );

    DDRB &= ~(1 << DDB4); // Set SPI MISO (Master In Slave Out) to input

    SPCR = (
        (1 << SPE) // SPI Enable
        | (1 << MSTR) // Set SPI Master mode
    ); // Enable SPI and set self as master

    SPCR &= ~(
        (1 << DORD) // Set SPI MSB
        | (1 << CPOL) // Set SPI leading clock edge to rising
        | (1 << CPHA) // Sample SPI data on leading edge of the clock
        | (1 << SPR1) // Spe
        | (1 << SPR0) //    ed
    ); // Set SPI bit order to MSB, sample data on leading clock edge and set the clock to 'fosc/4'.
    SPSR |= (1 << SPI2X); // Double SPI speed

    PORTB &= ~(1 << DDB2); // Set SPI CS (Chip Select) to active LOW
}


// Initialize SPI transfer by setting SPI CS (Chip Select) to active LOW
inline void DisplayClass::spi_begin() {
    PORTB &= ~(1 << DDB2); // Set SPI CS (Chip Select) to active LOW
}


// Finalize SPI transfer by setting SPI CS (Chip Select) to active HIGH
inline void DisplayClass::spi_end() {
    PORTB |= (1 << DDB2); // Set SPI CS (Chip Select) to active HIGH
}


// Transfer one byte over SPI
uint8_t DisplayClass::spi_transfer(uint8_t data) {
    SPDR = data; // Get transfer'd

    asm volatile("nop"); // Performing a nop speeds up the wait loop by about 10%... somehow.
    while(!(SPSR & (1 << SPIF))); // Hold program until SPI has been sent.

    return SPDR;
}


inline void DisplayClass::init_display_registers() {
    DDRB |= (1 << DDB1); // Set DC to output mode
}


void DisplayClass::begin() {
    this->spi_init();
    this->init_display_registers();
    this->start_display_startup_sequence();
}


// Send 8bit Display SPI command with no arguments
void DisplayClass::send_command(uint8_t command, bool end_spi_after_command = true) {
    this->spi_begin();
    PORTB &= ~(1 << PORTB1); // Set DC LOW
    this->spi_transfer(command);
    PORTB |= (1 << PORTB1); // Set DC HIGH
    if (end_spi_after_command) {
        this->spi_end();
    }
}


// Send 8bit Display SPI command with 8bit arguments
void DisplayClass::send_command(uint8_t command, uint8_t* args, uint32_t args_len) {
    this->spi_begin();
    PORTB &= ~(1 << PORTB1); // Set DC LOW
    this->spi_transfer(command);

    PORTB |= (1 << PORTB1); // Set DC HIGH
    for (uint32_t i = 0; i < args_len; i++) {
        this->spi_transfer(args[i]);
    }
    this->spi_end();
}


// Sets the address window for the pixels to be written to
void DisplayClass::set_address_window(uint16_t column_start, uint16_t row_start, uint16_t column_end, uint16_t row_end) {
    uint8_t params[DISPLAY_ADDRESS_PARAM_SIZE] = {
        (uint8_t)((column_start & 0xFF00) >> 8),
        (uint8_t)((column_start & 0x00FF)),
        (uint8_t)((column_end & 0xFF00) >> 8),
        (uint8_t)((column_end & 0x00FF)),
    };
    this->send_command(DISPLAY_COLUMN_ADDRESS_SET_COMMAND, params, DISPLAY_ADDRESS_PARAM_SIZE);

    params[0] = (row_start & 0xFF00) >> 8;
    params[1] = (row_start & 0x00FF);
    params[2] = (row_end & 0xFF00) >> 8;
    params[3] = (row_end & 0x00FF);
    this->send_command(DISPLAY_PAGE_ADDRESS_SET_COMMAND, params, DISPLAY_ADDRESS_PARAM_SIZE);
}


void DisplayClass::fill_screen(uint16_t color) {
    this->draw_rectangle(0, 0, DISPLAY_COLUMN_PIXEL_AMOUNT, DISPLAY_ROW_PIXEL_AMOUNT, color);
}


// Transfer pixel color over SPI
inline void DisplayClass::transfer_pixel_color_spi(uint16_t color) {
    /*
    A color takes up 16 bits, but the SPI only transfers 8 bits at a time
    So we split the 16-bit color up in to two 8 bit transfers
    */
    this->spi_transfer((color & 0xFF00) >> 8);
    this->spi_transfer(color & 0x00FF);
}


// Set the color of a given pixel
void DisplayClass::draw_pixel(uint16_t column, uint16_t row, uint16_t color) {
    this->set_address_window(column, row, column, row);

    this->send_command(DISPLAY_MEMORY_WRITE_COMMAND, false);
    this->transfer_pixel_color_spi(color);
    this->spi_end();
}


// Draw a rectangle in the given boundaries and color it the given color
void DisplayClass::draw_rectangle(
    uint16_t column_start,
    uint16_t row_start,
    uint16_t column_end,
    uint16_t row_end,
    uint16_t color
) {
    this->set_address_window(column_start, row_start, column_end, row_end);

    this->send_command(DISPLAY_MEMORY_WRITE_COMMAND, false);
    for (uint16_t col = column_start; col <= column_end; col += 1) {
        for (uint16_t row = row_start; row <= row_end; row += 1) {
            this->transfer_pixel_color_spi(color);
        }
    }
    this->spi_end();
}


// Draw a circle with a given radius and color. The circle expands from the given coordinates
void DisplayClass::draw_circle(
    uint16_t column,
    uint16_t row,
    uint16_t radius,
    uint16_t color
) {
    uint16_t column_min = max(column - radius, 0);
    uint16_t column_max = min(column + radius, DISPLAY_COLUMN_PIXEL_AMOUNT);
    uint16_t row_min = max(row - radius, 0);
    uint16_t row_max = min(row + radius, DISPLAY_ROW_PIXEL_AMOUNT);

    this->draw_pixel(column, row, color);
    this->draw_pixel(column_min, row, color);
    this->draw_pixel(column_max, row, color);
    this->draw_pixel(column, row_min, color);
    this->draw_pixel(column, row_max, color);
}


DisplayClass Display = DisplayClass();
