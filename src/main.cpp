#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>


#define NUNCHUCK_TWI_ADDR 0x52

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


void init_registers() {

}


void init_twi() {
    Wire.begin();
}


void initialize() {
    sei();
#if NUNCHUCK_DEBUG
    Serial.begin(9600);
#endif
    init_registers();
    init_twi();
    Display.begin();
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
}


int main() {
    initialize();

    Display.show_square();
    // Display.fill_screen(0xF8F8);

    Display.draw_rect(100, 110, 100, 110, 0xFFFF);
    Display.draw_rect(90, 100, 90, 100, 0xFFFF);

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
