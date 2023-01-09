#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>

#define NUNCHUCK_TWI_ADDR 0x52

// colors
#define COLOR_RED 0xF800
#define COLOR_ORANGE 0xFC00
#define COLOR_YELLOW 0xFFE0
#define COLOR_GREEN 0x07E0
#define COLOR_LIGHT_GRAY 0xBDF7
#define COLOR_GRAY 0x8410
#define COLOR_DARK_GRAY 0x4208
#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "Monster/Monster.h"
#include "sprites/font.h"
#include "sprites/test.h"
#include "sprites/slime.h"

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

    Display.fill_screen(COLOR_BLACK);

    uint8_t stage = 0;
    while (true) {
        switch(stage) {
            case 0:
                // main menu
                Display.draw_centered_text("Game Name(tm)", 130, FONT_SPRITES, 3, COLOR_RED);
                Display.draw_centered_text("PRESS Z TO START", 100, FONT_SPRITES, 2, COLOR_LIGHT_GRAY);
                while (true) {
                    Nunchuck.fetch_state();

                    if (!Nunchuck.state_changed()) continue;

                    if (Nunchuck.current_state.z_pressed) {
                        stage = 1;
                        break;
                    }
                    else if (Nunchuck.current_state.c_pressed && !Nunchuck.current_state.z_pressed) {
                        for (uint8_t i = 0; i < 8; i++) {
                            Display.invert_colors();
                            _delay_ms(75);
                        }
                    }
                }
                break;
            case 1:
                uint16_t right_moves_x = floor(DISPLAY_ROW_PIXEL_AMOUNT/2);
                Display.fill_screen(COLOR_GREEN);
                Display.draw_rectangle(0, 0, DISPLAY_ROW_PIXEL_AMOUNT-1, 90, COLOR_WHITE);
                Display.draw_border(0, 0, DISPLAY_ROW_PIXEL_AMOUNT-1, 90, 6, 0, COLOR_BLACK);
                Display.draw_border(3, 3, DISPLAY_ROW_PIXEL_AMOUNT-4, 87, 3, 1, COLOR_DARK_GRAY);
                Display.draw_border(6, 6, DISPLAY_ROW_PIXEL_AMOUNT-7, 84, 6, 0, COLOR_DARK_GRAY);
                Display.draw_border(9, 9, DISPLAY_ROW_PIXEL_AMOUNT-10, 81, 3, 1, COLOR_WHITE);

                // Move slime_moveset
                Monster slime = Monster(
                    "SLIME",
                    {DISSOLVE, POUND, SHIELD, QUICK_ATTACK_MOVE},
                    SLIME_SPRITE,
                    SLIME_COLORS,
                    10
                );

                Display.draw_text("POUND", 20, 60, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_text("POW:20 ACC:60%", 20, 50, FONT_SPRITES, 1, COLOR_DARK_GRAY);

                Display.draw_text("ABSORB", 20, 30, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_text("POW:8 ACC:70%", 20, 20, FONT_SPRITES, 1, COLOR_DARK_GRAY);

                Display.draw_border(16, 16, right_moves_x-5, 47, 2, 1, COLOR_RED);

                Display.draw_text("SHIELD", right_moves_x, 60, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_text("DEF:100 ACC:80%", right_moves_x, 50, FONT_SPRITES, 1, COLOR_DARK_GRAY);

                Display.draw_text("QUICK ATTACK", right_moves_x, 30, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_text("POW:10 ACC:95%", right_moves_x, 20, FONT_SPRITES, 1, COLOR_DARK_GRAY);

                while (true) {
                    Nunchuck.fetch_state();

                    if (!Nunchuck.state_changed()) continue;

                    else if (Nunchuck.current_state.c_pressed && !Nunchuck.current_state.z_pressed) {
                        for (uint8_t i = 0; i < 8; i++) {
                            Display.invert_colors();
                            _delay_ms(75);
                        }
                    }
                }
                break;
        }
    }

    // for (uint16_t x = 0; x <= 305; x += 16) {
    //     for (uint16_t y = 0; y <= 225; y += 16) {
    //         Display.draw_sprite(TEST_SPRITE, TEST_COLORS, x, y, 3);
    //     }
    // }


    // Display.fill_screen(0x0000);
    // Display.draw_border(2, 2, DISPLAY_ROW_PIXEL_AMOUNT-2, DISPLAY_COLUMN_PIXEL_AMOUNT-2, 3, 1, 0x07E0);

    // Display.draw_sprite(SLIME_SPRITE, SLIME_COLORS, 50, 100, 5);
    // _delay_ms(1000);
    // Display.draw_text("CraftOS 1.8\n> ", 10, 210, FONT_SPRITES, 2 ,0xDEED);
    // _delay_ms(1000);
    // Display.draw_text("\n  Hello there!", 10, 210, FONT_SPRITES, 2);
    // _delay_ms(1000);
    // while (true) {
    //     _delay_ms(500);
    //     Display.draw_text("\n              _", 10, 210, FONT_SPRITES, 2, 0x0000);
    //     _delay_ms(500);
    //     Display.draw_text("\n              _", 10, 210, FONT_SPRITES, 2);
    // }



    /*uint16_t square_color;
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

        //Display.draw_rectangle(100, 100, 132, 132, square_color);
    }*/

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
