#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

#include "sprites.h"
#include "Move.h"

#define MONSTER_MAX_MOVE_AMOUNT 4


enum MonsterKind {
    SLIME_MONSTER,
    FIRE_SLIME_MONSTER,
    KITTY_MONSTER,
};


class Monster {
    public:
        Monster(MonsterKind monster, const Move* moveset);
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
