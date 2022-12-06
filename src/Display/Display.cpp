#include "Display.h"
#ifndef DDRB
#include <avr/io.h>
#endif

#include <util/delay.h>


DisplayClass::DisplayClass() {}


void DisplayClass::startup() {
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


void DisplayClass::register_init() {
    DDRB |= (1 << DDB1);
}


void DisplayClass::begin() {
    this->spi_settings = SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0);
    SPI.begin();
    this->register_init();

    this->startup();
}


void DisplayClass::send_command(uint8_t command) {
    SPI.beginTransaction(this->spi_settings);

    PORTB &= ~(1 << PORTB1); // Set DC LOW
    SPI.transfer(command);

    SPI.endTransaction();
}


void DisplayClass::send_command(uint8_t command, uint8_t* args, uint8_t args_len) {
    SPI.beginTransaction(this->spi_settings);

    PORTB &= ~(1 << PORTB1); // Set DC LOW
    SPI.transfer(command);

    PORTB |= (1 << PORTB1); // Set DC HIGH
    for (uint8_t i = 0; i < args_len; i++) {
        SPI.transfer(args[i]);
    }

    SPI.endTransaction();
}


void DisplayClass::show_square() {
    // Send data via SPI to select a square and fill it with a simple byte-based data. (Effective for drawing 1 color UI elements or smaller stored sprites.)
}

<<<<<<< HEAD
int Display::SPI_setup() {
    // Assuming SPI_DDR = DDRB, set output correctly.
    DDR_SPI |= (1 << CS) | (1 << MOSI) | (1 << SCK);
    // Enable SPI, set self as master, and set the clock to 'fosc/128'.
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);

    // SPI pins should now be ready. Writing data into SPDR should make it auto-trigger & sent said data over SPI.

    // Since there's only 1 SPI slave device, we set CS to low by standard.
    DDR_SPI &= ~(1 << CS);
}

uint8_t Display::SPI_send(uint8_t data) {
    SPDR = data; // Send data into SPDR, ATMega will automatically dial it out.

    // Hold program until SPI has been send.
    while(!(SPSR & (1<<SPIF)));

    return SPDR;
}
=======

DisplayClass Display = DisplayClass();
>>>>>>> Setup DisplayClass
