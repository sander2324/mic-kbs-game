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
#define COLOR_WHITE 0xFFFE
#define COLOR_BLACK 0x0000

#define CHAR_WIDTH 6

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "Monster/Monster.h"
#include "Monster/monster_moves.h"
#include "sprites/font.h"
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

void draw_move(Move move, uint16_t x, uint16_t y) {
    Display.draw_text(move.name, x, y, FONT_SPRITES, 2, COLOR_BLACK);
    char buffer [sizeof(int)*8+1];
    if (move.power > 0) {
        itoa(move.power, buffer, 10);
        Display.draw_text("POW:", x, y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
    } else {
        //Display.draw_text("DEF:" + itoa(move.defense) + " ACC:" + to_string(move.accuracy), 20, y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
        itoa(move.defense, buffer, 10);
        Display.draw_text("DEF:", x, y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
    }
    Display.draw_text(buffer, x + (CHAR_WIDTH * 4), y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
    Display.draw_text("ACC:", x + (CHAR_WIDTH * 7), y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
    itoa(move.accuracy, buffer, 10);
    Display.draw_text(buffer, x + (CHAR_WIDTH * 11), y-10, FONT_SPRITES, 1, COLOR_DARK_GRAY);
}

void draw_health_bar(Monster monster, uint16_t x, uint16_t y, uint16_t width) {
    uint16_t color = COLOR_GREEN;
    if (monster.current_health < monster.max_health/5) {
        color = COLOR_RED;
    } else if (monster.current_health < monster.max_health/2) {
        color = COLOR_YELLOW;
    }
    Display.draw_rectangle(x, y-6, x+width-1, y, COLOR_GRAY);
    Display.draw_rectangle(x, y-6, x+floor(width*((float)monster.current_health/(float)monster.max_health))-1, y, color);
}

void draw_monster_info(Monster monster, uint16_t x, uint16_t y) {
    Display.draw_rectangle(x+4, y-46, x+146, y-4, COLOR_WHITE);
    Display.draw_rectangle(x+4, y-29, x+146, y-28, COLOR_RED);
    Display.draw_rectangle(x+4, y-46, x+146, y-31, COLOR_RED);
    Display.draw_border(x,y-50, x+150, y, 4, 1, COLOR_BLACK);
    Display.draw_text(monster.name, x+8, y-20, FONT_SPRITES, 2, COLOR_BLACK);
    Display.draw_rectangle(x+30, y-35, x+127, y-27, COLOR_DARK_GRAY);
    Display.draw_text("HP", x+32, y-34, FONT_SPRITES, 1, 0xFFFE);
    Display.draw_border(x+29, y-38, x+130, y-24, 3, 1, COLOR_DARK_GRAY);
    draw_health_bar(monster, x+46, y-28, 80);
}

void wait_for_z() {
    while (true) {
        Nunchuck.fetch_state();

        if (!Nunchuck.state_changed()) continue;

        if (Nunchuck.current_state.z_pressed) {
            return;
        }
    }
}

void concat(char* string_one, char* string_two) {
    uint8_t len = 0;
    for (uint8_t i = 0; string_one[i] != '\0'; i++) {
        len++;
    }
    for (uint8_t i = 0; string_two[i] != '\0'; i++) {
        string_one[len] = string_two[i];
        len++;
    }
}

int main() {
    Move QUICK_ATTACK_MOVE = Move("QUICK ATK",15,90,0);
    Move POUND_MOVE = Move("POUND",40,40,0);
    Move SHIELD_MOVE = Move("SHIELD",0,70,100);
    Move JUMP_MOVE = Move("JUMP",0,80,90);
    Move DISSOLVE_MOVE = Move("DISSOLVE",30,80,0);
    Move FIRE_BREATH_MOVE = Move("FIRE BREATH",30,80,0);

    uint8_t move_selected = 0;

    initialize();

    Display.fill_screen(COLOR_BLACK);

    enum GameState {
        MAIN_MENU,
        START_BATTLE,
        RENDER_MENU,
        SELECT_MOVE,
        WAIT_FOR_OPPONENT,
        FIRST_MOVE,
        FIRST_MISS,
        SECOND_MOVE,
        SECOND_MISS,
        DEATH,
        WIN,
    };

    GameState stage = MAIN_MENU;
    Move slime_moveset[] = {POUND_MOVE, JUMP_MOVE, QUICK_ATTACK_MOVE, DISSOLVE_MOVE};
    Move fire_slime_moveset[] = {POUND_MOVE, JUMP_MOVE, FIRE_BREATH_MOVE, DISSOLVE_MOVE};
    Monster player_one = Monster(MonsterKind::SLIME_MONSTER, slime_moveset);
    Monster player_two = Monster(MonsterKind::FIRE_SLIME_MONSTER, fire_slime_moveset);
    uint16_t right_moves_x_offset = floor(DISPLAY_ROW_PIXEL_AMOUNT/2)-20;
    uint16_t bottom_moves_y_offset = 30;
    while (true) {
        switch(stage) {
            case DEATH:
                for (uint8_t i = 0; i < 16; i++) {
                    Display.invert_colors();
                    _delay_ms(75);
                }
                Display.fill_screen(COLOR_BLACK);
                Display.draw_centered_text("YOU LOSE",130, FONT_SPRITES, 3, COLOR_RED);
                Display.draw_centered_text("PRESS Z TO RESTART", 100, FONT_SPRITES, 2, COLOR_LIGHT_GRAY);
                while (true) {
                    Nunchuck.fetch_state();
                    if (!Nunchuck.state_changed()) continue;
                    if (Nunchuck.current_state.z_pressed) {
                        stage = START_BATTLE;
                        break;
                    }
                }
                break;
            case WIN:
                _delay_ms(500);
                for (uint8_t i = 0; i < 16; i++) {
                    Display.invert_colors();
                    _delay_ms(75);
                }
                Display.fill_screen(COLOR_BLACK);
                Display.draw_centered_text("YOU WIN",130, FONT_SPRITES, 3, COLOR_GREEN);
                Display.draw_centered_text("PRESS Z TO RESTART", 100, FONT_SPRITES, 2, COLOR_LIGHT_GRAY);
                while (true) {
                    Nunchuck.fetch_state();
                    if (!Nunchuck.state_changed()) continue;
                    if (Nunchuck.current_state.z_pressed) {
                        stage = START_BATTLE;
                        break;
                    }
                }
                break;
            case MAIN_MENU:
                // main menu
                Display.draw_centered_text("Game Name(tm)", 130, FONT_SPRITES, 3, COLOR_RED);
                Display.draw_centered_text("PRESS Z TO START", 100, FONT_SPRITES, 2, COLOR_LIGHT_GRAY);
                _delay_ms(500);
                wait_for_z();
                stage = START_BATTLE;

            case START_BATTLE:
                Display.fill_screen(COLOR_GREEN);
                //Move slime_moveset[] = {DISSOLVE_MOVE, POUND_MOVE, SHIELD_MOVE, QUICK_ATTACK_MOVE};
                player_one = Monster(MonsterKind::SLIME_MONSTER, slime_moveset);
                player_two = Monster(MonsterKind::FIRE_SLIME_MONSTER, fire_slime_moveset);
                right_moves_x_offset = floor(DISPLAY_ROW_PIXEL_AMOUNT/2)-20;
                bottom_moves_y_offset = 30;

                Display.draw_sprite(player_one.sprite, player_one.sprite_colors, 25, 100, 5);

                Display.draw_sprite(player_two.sprite, player_two.sprite_colors, 250, 150, 5, true);


                draw_monster_info(player_one, DISPLAY_ROW_PIXEL_AMOUNT-150, DISPLAY_COLUMN_PIXEL_AMOUNT-100);

                draw_monster_info(player_two, 0, DISPLAY_COLUMN_PIXEL_AMOUNT-10);

                stage = RENDER_MENU;

            case RENDER_MENU:
                Display.draw_border(0, 0, DISPLAY_ROW_PIXEL_AMOUNT-1, 90, 6, 0, COLOR_BLACK);
                Display.draw_border(3, 3, DISPLAY_ROW_PIXEL_AMOUNT-4, 87, 3, 1, COLOR_DARK_GRAY);
                Display.draw_border(6, 6, DISPLAY_ROW_PIXEL_AMOUNT-7, 84, 6, 0, COLOR_DARK_GRAY);
                Display.draw_border(9, 9, DISPLAY_ROW_PIXEL_AMOUNT-10, 81, 3, 1, COLOR_WHITE);
                stage = SELECT_MOVE;

            case SELECT_MOVE:
                Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);

                draw_move(player_one.moveset[0], 20, 60);
                draw_move(player_one.moveset[2], 20, 60-bottom_moves_y_offset);

                draw_move(player_one.moveset[1], 20+right_moves_x_offset, 60);
                draw_move(player_one.moveset[3], 20+right_moves_x_offset, 60-bottom_moves_y_offset);


                Display.draw_border(
                    16 + ((move_selected % 2) * right_moves_x_offset),
                    16 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                    (20+right_moves_x_offset) - 5 + ((move_selected % 2) * right_moves_x_offset),
                    (60-bottom_moves_y_offset) + 17 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                    2,
                    1,
                    COLOR_RED
                );


                Display.draw_border(
                    16 + ((move_selected % 2) * right_moves_x_offset),
                    16 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                    (20+right_moves_x_offset) - 5 + ((move_selected % 2) * right_moves_x_offset),
                    (60-bottom_moves_y_offset) + 17 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                    2,
                    1,
                    COLOR_RED
                ); // why is this here? good question. try removing it and selecting a move without toggling the joystick. that's why it's here.

                while (true) {
                    Nunchuck.fetch_state();

                    if (!Nunchuck.state_changed()) continue;

                    if (Nunchuck.current_state.z_pressed && !Nunchuck.current_state.c_pressed) {
                        /*slime.current_health = slime.current_health-10;
                        draw_health_bar(slime, 46, DISPLAY_COLUMN_PIXEL_AMOUNT-20-28, 80);
                        if (slime.current_health <= 0) {
                            for (uint8_t i = 0; i < 16; i++) {
                                Display.invert_colors();
                                _delay_ms(75);
                            }
                            Display.fill_screen(COLOR_BLACK);
                            Display.draw_centered_text("YOU LOSE",130, FONT_SPRITES, 3, COLOR_RED);
                            Display.draw_centered_text("PRESS Z TO RESTART", 100, FONT_SPRITES, 2, COLOR_LIGHT_GRAY);
                            while (true) {
                                Nunchuck.fetch_state();
                                if (!Nunchuck.state_changed()) continue;
                                if (Nunchuck.current_state.z_pressed) {
                                    stage = 1;
                                    break;
                                }
                            }
                            break;
                        }*/
                        stage = WAIT_FOR_OPPONENT;
                        break;
                    } else if (Nunchuck.current_state.joystick_state != Nunchuck.previous_state.joystick_state) {
                        Display.draw_border(
                            16 + ((move_selected % 2) * right_moves_x_offset),
                            16 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                            (20+right_moves_x_offset) - 5 + ((move_selected % 2) * right_moves_x_offset),
                            (60-bottom_moves_y_offset) + 17 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                            2,
                            1,
                            COLOR_WHITE
                        );
                        if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::UP || Nunchuck.current_state.joystick_state == NunchuckJoystickState::DOWN) {
                            move_selected = move_selected + 2;
                        } else if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::RIGHT) {
                            move_selected = move_selected + 1;
                        } else if (Nunchuck.current_state.joystick_state == NunchuckJoystickState::LEFT) {
                            move_selected = move_selected - 1;
                        }
                        move_selected = move_selected%4;
                        Display.draw_border(
                            16 + ((move_selected % 2) * right_moves_x_offset),
                            16 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                            (20+right_moves_x_offset) - 5 + ((move_selected % 2) * right_moves_x_offset),
                            (60-bottom_moves_y_offset) + 17 + (bottom_moves_y_offset - (floor(move_selected / 2) * bottom_moves_y_offset)),
                            2,
                            1,
                            COLOR_RED
                        );
                    }
                }
            case WAIT_FOR_OPPONENT:
                Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);
                Display.draw_text("WAITING FOR OPPONENT...", 20, 40, FONT_SPRITES, 2, COLOR_BLACK);
                _delay_ms(500);
                uint16_t wait_time = 0;

                if (wait_time < 1) {
                    // etc
                }
            case FIRST_MOVE:

                Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);
                Display.draw_centered_text("YOU USED", 50, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_centered_text(player_one.moveset[move_selected].name, 30, FONT_SPRITES, 2, COLOR_BLACK);

                if ((int)rand()%101 <= player_one.moveset[move_selected].accuracy) {
                    if (player_one.moveset[move_selected].power > 0) {
                        player_two.current_health = player_two.current_health-player_one.moveset[move_selected].power;
                        draw_health_bar(player_two, 46, DISPLAY_COLUMN_PIXEL_AMOUNT-10-28, 80);
                        if (player_two.current_health <= 0) {
                            stage = WIN;
                            break;
                        } else {
                            _delay_ms(500);
                            wait_for_z();
                            stage = SECOND_MOVE;
                        }
                    }
                } else {
                    _delay_ms(500);
                    wait_for_z();
                    stage = FIRST_MISS;
                }

                // stage = SELECT_MOVE;
                // break;
                /*char* text = "YOUR ";
                uint8_t len = 5;
                for (uint8_t i = 0; slime.moveset[move_selected].name[i] != '\0'; i++) {
                    text[len] = slime.moveset[move_selected].name[i]
                Display.draw_text(slime.moveset[move_selected].name, )*/

            case FIRST_MISS:
                if (stage == FIRST_MISS) {
                    Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);
                    Display.draw_centered_text("BUT IT MISSED!", 40, FONT_SPRITES, 2, COLOR_BLACK);
                    _delay_ms(500);
                    wait_for_z();
                    stage = SECOND_MOVE;
                }

            case SECOND_MOVE:
                uint8_t enemy_move = rand()%4;
                Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);
                Display.draw_centered_text("ENEMY USED", 50, FONT_SPRITES, 2, COLOR_BLACK);
                Display.draw_centered_text(player_two.moveset[enemy_move].name, 30, FONT_SPRITES, 2, COLOR_BLACK);
                // Display.draw_centered_text("RIP", 50, FONT_SPRITES, 2, COLOR_BLACK);

                if ((int)rand()%101 <= player_two.moveset[enemy_move].accuracy) {
                    if (player_two.moveset[enemy_move].power > 0) {
                        for (uint8_t i = 0; i < 2; i++) {
                            Display.invert_colors();
                            _delay_ms(75);
                        }
                        player_one.current_health = player_one.current_health-player_two.moveset[enemy_move].power;
                        draw_health_bar(player_one, DISPLAY_ROW_PIXEL_AMOUNT-150 + 46, DISPLAY_COLUMN_PIXEL_AMOUNT-100-28, 80);
                        if (player_one.current_health <= 0) {
                            stage = DEATH;
                            break;
                        } else {
                            _delay_ms(500);
                            wait_for_z();
                            stage = SELECT_MOVE;
                            break;
                        }
                    }
                } else {
                    wait_for_z();
                    stage = SECOND_MISS;
                    _delay_ms(500);
                }

            case SECOND_MISS:
                Display.draw_rectangle(12, 12, DISPLAY_ROW_PIXEL_AMOUNT-13, 78, COLOR_WHITE);
                Display.draw_centered_text("BUT IT MISSED!", 40, FONT_SPRITES, 2, COLOR_BLACK);
                _delay_ms(500);
                wait_for_z();
                stage = SELECT_MOVE;
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
