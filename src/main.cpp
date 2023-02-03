#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>

#include <HardwareSerial.h>


#define NUNCHUCK_TWI_ADDR 0x52

#include "global_vars.h"

#include "SuperCounter/SuperCounter.h"
#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "Monster/Monster.h"
#include "Monster/Move.h"
#include "sprites/font.h"
#include "sprites/test.h"
//#include "PotMeter/PotMeter.h"
#include "PersistentStorage/PersistentStorage.h"
#include "Battle/Battle.h"
#include "Infrared/Infrared.h"
#include "SegmentDisplay/SegmentDisplay.h"

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


inline void init_twi() {
    Wire.begin();
}


void initialize() {
#if NUNCHUCK_DEBUG
    Serial.begin(9600);
#endif
    init_supercounter();
    init_twi();

    sei();

    Display.begin();
    PersistentStorage.begin();
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
    //PotMeter.prepareADC();
}


int main() {
    initialize();
    Serial.begin(9600);

    Display.fill_screen(COLOR_WHITE);
    Display.draw_centered_text("SELECT FREQUENCY",200, FONT_SPRITES, FONT_FG_BLACK_BG_TRANS, 2);
    Display.draw_centered_text("38kHz", 130, FONT_SPRITES, FONT_FG_GREEN_BG_TRANS, 3);
    Display.draw_centered_text("56kHz", 80, FONT_SPRITES, FONT_FG_DARK_GRAY_BG_TRANS, 2);

    uint8_t frequency = 0;

    segment_display_char(frequency);

    while (true) {
        Nunchuck.fetch_state();

        if (!Nunchuck.state_changed()) continue;

        if (Nunchuck.current_state.z_pressed && !Nunchuck.current_state.c_pressed) {
            Display.fill_screen(COLOR_BLACK);
            break;
        } else if (Nunchuck.current_state.joystick_state != Nunchuck.previous_state.joystick_state) {
            if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::UP || Nunchuck.current_state.joystick_state == NunchuckJoystickState::DOWN) {
                Display.draw_centered_text("38kHz", 130, FONT_SPRITES, FONT_FG_WHITE_BG_TRANS, 3-frequency);
                Display.draw_centered_text("56kHz", 80, FONT_SPRITES, FONT_FG_WHITE_BG_TRANS, 2+frequency);
                frequency = (frequency+1)%2;
                segment_display_char(frequency);
                if (frequency == 1) {
                    Display.draw_centered_text("38kHz", 130, FONT_SPRITES, FONT_FG_DARK_GRAY_BG_TRANS, 2);
                    Display.draw_centered_text("56kHz", 80, FONT_SPRITES, FONT_FG_GREEN_BG_TRANS, 3);
                } else {
                    Display.draw_centered_text("38kHz", 130, FONT_SPRITES, FONT_FG_GREEN_BG_TRANS, 3);
                    Display.draw_centered_text("56kHz", 80, FONT_SPRITES, FONT_FG_DARK_GRAY_BG_TRANS, 2);
                }
            }
        }
    }

    ir_setup(frequency); // will it CRASH or will it WORK?!

    Display.fill_screen(COLOR_WHITE);
    Display.draw_centered_text("PAIRING...", 200, FONT_SPRITES, FONT_FG_DARK_GRAY_BG_TRANS, 3);

    int last_y = 200;
    if (frequency == 0) {
        send_ir(0b1010110101);
    } else {
        //send_ir(0b1110110111);
    }
    while (true)
    {
        if (frequency == 0) {
            uint16_t irdata = receive_ir();
            if (irdata != 0) {
                char buffer [sizeof(int)*8+1];
                itoa(irdata, buffer, 1);
                last_y += 15;
                Display.draw_centered_text(buffer, last_y, FONT_SPRITES, FONT_FG_LIGHT_GRAY_BG_TRANS, 2);
            }
        } else {
            uint16_t irdata = receive_ir();
            if (irdata == 0b1010110101) {
                last_y += 15;
                Display.draw_centered_text("Oh my god", last_y, FONT_SPRITES, FONT_FG_GREEN_BG_TRANS, 2);
            } else {
                char buffer [sizeof(int)*8+1];
                itoa(irdata, buffer, 1);
                last_y += 15;
                Display.draw_centered_text(buffer, last_y, FONT_SPRITES, FONT_FG_LIGHT_GRAY_BG_TRANS, 2);
            }
        }
    }


#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
