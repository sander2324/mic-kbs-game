#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>

#define NUNCHUCK_TWI_ADDR 0x52

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "sprites/test.h"

#include <HardwareSerial.h>

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

    // while (true) {
    //     // Display.fill_screen(0xFFFF);
    //     // Display.draw_rectangle(5, 5, 132, 5, 0xFFFF);
    //     // Display.draw_rectangle(5, 10, 132, 10, 0xFFFF);
    //     // Display.draw_rectangle(5, 15, 132, 15, 0xFFFF);
    //     // Display.draw_rectangle(5, 20, 132, 20, 0xFFFF);
    //     // Display.draw_rectangle(5, 25, 132, 25, 0xFFFF);

    //     // Display.draw_rectangle(100, 100, 150, 150, 0x0000);
    //     // Display.draw_rectangle(175, 100, 175 + 50, 150, 0x0001);

    //     // Display.draw_circle(200, 200, 15, 0xFFFF);
    //     // _delay_ms(500);

    //     Display.fill_screen(0xFFFF);
    //     _delay_ms(500);

    //     Display.fill_screen(0xFFFE);
    //     _delay_ms(500);
    // }

    Display.fill_screen(0xF8F8);

    Serial.begin(9600);

    uint16_t square_color;
    Display.draw_sprite(TEST_SPRITE, 150, 150);
    while (true) {
        Nunchuck.set_state();

        if (!Nunchuck.state_changed()) continue;

        square_color = 0xFFFF;
        if (Nunchuck.current_state.z_pressed && !Nunchuck.current_state.c_pressed) {
            square_color = 0xEEEE;
        }
        else if (Nunchuck.current_state.c_pressed && !Nunchuck.current_state.z_pressed) {
            for (uint8_t i = 0; i < 8; i++) {
                Display.invert_colors();
                _delay_ms(75);
            }
        }
        else if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::UP) {
            square_color = 0x3E92;
        }
        else if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::DOWN) {
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
