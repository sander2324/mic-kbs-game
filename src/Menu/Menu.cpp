#include "Menu.h"


MenuClass::MenuClass() {};


void MenuClass::draw_title() {
    Display.fill_screen(COLOR_RED);
    Display.fill_screen(COLOR_GREEN);
    Display.fill_screen(COLOR_LIGHT_GRAY);
    Display.fill_screen(COLOR_BLACK);
    Display.draw_centered_text("TASMON(TM)", 130, FONT_SPRITES, FONT_FG_RED_BG_TRANS, 3);
    Display.draw_centered_text("PRESS Z TO START", 100, FONT_SPRITES, FONT_FG_LIGHT_GRAY_BG_TRANS, 2);
    _delay_ms(500);
    Nunchuck.wait_for_z();
};


MenuClass Menu = MenuClass();
