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

    Display.fill_screen(0xF8F8);
    Display.draw_rectangle(5, 5, 5, 132, 0xFFFF);
    Display.draw_rectangle(10, 5, 10, 132, 0xFFFF);
    Display.draw_rectangle(15, 5, 15, 132, 0xFFFF);
    Display.draw_rectangle(20, 5, 20, 132, 0xFFFF);
    Display.draw_rectangle(25, 5, 25, 132, 0xFFFF);

    Display.draw_circle(200, 200, 5, 0xFFFF);

    uint16_t square_color;
    while (true) {
        Nunchuck.set_state();

        if (!Nunchuck.state_changed()) continue;

        square_color = 0xFFFF;
        if (Nunchuck.current_state.z_pressed) {
            square_color = 0xEEEE;
        }
        else if (Nunchuck.current_state.c_pressed) {
            square_color = 0xDDDE;
        }

        Display.draw_rectangle(100, 100, 132, 132, square_color);
    }

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
