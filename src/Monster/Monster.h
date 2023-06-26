#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>
#include <stdlib.h>

#include "sprites.h"
#include "Move.h"

#define MONSTER_MAX_MOVE_AMOUNT 4
#define MONSTER_KIND_COUNT 3


enum MonsterKind {
    SLIME_MONSTER = 0,
    FIRE_SLIME_MONSTER = 1,
    KITTY_MONSTER = 2,
};


class Monster {
    public:
        Monster(MonsterKind monster);
        static Monster get_random();
        MonsterKind kind;
        const char* name;
        const Move* moveset;
        const uint8_t* sprite;
        const uint16_t* sprite_colors;
        uint8_t max_health;
        int8_t current_health;
};


const Move SLIME_MOVESET[] = { POUND_MOVE, JUMP_MOVE, QUICK_ATTACK_MOVE, DISSOLVE_MOVE };
const Move FIRE_SLIME_MOVESET[] = { POUND_MOVE, JUMP_MOVE, FIRE_BREATH_MOVE, DISSOLVE_MOVE };
const Move KITTY_MOVESET[] = { POUNCE_MOVE, FELINE_FRENZY_MOVE, WHISKER_WHIP_MOVE, SCRATCH_MOVE };

#endif
