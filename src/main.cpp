#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>

#define NUNCHUCK_TWI_ADDR 0x52

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "sprites/test.h"

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

    Display.draw_circle(190, 190, 15, 0xFFFF);
    for (uint16_t x = 0; x <= 305; x += 16) {
        for (uint16_t y = 0; y <= 225; y += 16) {
            Display.draw_sprite(TEST_SPRITE, TEST_COLORS, x, y, 3);
        }
    }

    Display.draw_sprite(TEST_SPRITE, TEST_COLORS, 150, 150, 1);

    Display.fill_screen(0xF8F8);
    Display.draw_sprite(TEST_SPRITE, TEST_COLORS, 50, 50, 3);

    uint16_t square_color;
    while (true) {
        Nunchuck.fetch_state();

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
