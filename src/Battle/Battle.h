#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>
#include <math.h>

#include "../global_vars.h"

#include "../sprites/font.h"
#include "../Display/Display.h"
#include "../Monster/Monster.h"
#include "../Monster/Move.h"
#include "../Nunchuck/Nunchuck.h"

#define BOTTOM_MOVES_Y_OFFSET 30

const uint16_t RIGHT_MOVES_X_OFFSET = floor(DISPLAY_ROW_PIXEL_AMOUNT / 2) - 20;


struct BattlePlayer {
    const char* name;
    Monster monster;
    uint16_t most_consecutive_wins;
};


enum BattleState {
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


class Battle {
    public:
        Battle(BattlePlayer user, BattlePlayer opponent);
        void loop();
        void tick();

    private:
        BattleState stage = START_BATTLE;
        uint8_t move_selected = 0;
        BattlePlayer user;
        BattlePlayer opponent;
};


#endif
