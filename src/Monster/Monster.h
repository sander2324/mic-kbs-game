#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

#define MONSTER_MAX_MOVE_AMOUNT 4



struct Move {
    const char* name;
    const uint8_t power : 5;
    const uint8_t accuracy : 4;
    const uint8_t defense : 4;
};


class Monster {
    public:
        Monster(
            const char* name,
            Move* moveset,
            uint8_t* sprite,
            uint16_t* sprite_colors,
            const uint8_t max_health
        );
        const char* name;
        Move* moveset;
        uint8_t* sprite;
        uint16_t* sprite_colors;
        uint8_t max_health;
        uint8_t current_health;

};


Move QUICK_ATTACK_MOVE = {
    "QUICK ATTACK",
    8,
    8,
    0,
};

Move POUND = {
    "POUND",
    16,
    4,
    0,
};

Move SHIELD = {
    "SHIELD",
    0,
    7,
    100,
};

Move DISSOLVE = {
    "DISSOLVE",
    12,
    7,
    0,
};

#endif
