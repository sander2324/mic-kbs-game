#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

#include "sprites.h"

#define MONSTER_MAX_MOVE_AMOUNT 4


enum MonsterKind {
    SLIME_MONSTER,
};


struct Move {
    const char* name;
    const uint8_t power;
    const uint8_t accuracy;
    const uint8_t defense;
};


class Monster {
    public:
        Monster(MonsterKind monster);
        MonsterKind kind;
        const char* name;
        Move* moveset;
        const uint8_t* sprite;
        const uint16_t* sprite_colors;
        uint8_t max_health;
        uint8_t current_health;
        const uint8_t* get_sprite();
        const uint16_t* get_sprite_colors();

};

#endif
