#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

#include "sprites.h"
#include "monster_moves.h"

#define MONSTER_MAX_MOVE_AMOUNT 4

enum MonsterKind {
    SLIME_MONSTER,
    FIRE_SLIME_MONSTER,
    KITTY_MONSTER,
};

class Monster {
    public:
        Monster(MonsterKind monster, Move* moveset);
        MonsterKind kind;
        const char* name;
        const Move* moveset;
        const uint8_t* sprite;
        const uint16_t* sprite_colors;
        uint8_t max_health;
        int8_t current_health;
};

#endif
