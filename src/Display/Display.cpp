#include "Display.h"
#ifndef DDRB
#include <avr/io.h>
#endif

#ifndef PRId8 // Test if inttypes has been included somewhere else.
#include <inttypes.h>
#endif

#define DDR_SPI DDRB // DDRB register as SPI pins.
#define CS PINB2 // Chip Select is pin B2.
#define MOSI PINB3 // MOSI is pin B3.
#define MISO PINB4 // MISO is pin B4.
#define SCK PINB5 // Slave Clock is pin B5.

Display::Display() {
    // TODO
}

void Display::Display_Init() {
    // Send starting parameters & all such data.
}

void Display::Display_ShowSquare() {
    // Send data via SPI to select a square and fill it with a simple byte-based data. (Effective for drawing 1 color UI elements or smaller stored sprites.)
}

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
