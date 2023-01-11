#ifndef MOVES_H
#define MOVES_H

#include <stdint.h>

class Move {
    public:
        Move(char* name, uint8_t power, uint8_t accuracy, uint8_t defense);
        const char* name;
        uint8_t power;
        uint8_t accuracy;
        uint8_t defense;
};

#endif


/*Move QUICK_ATTACK_MOVE = {
    "QUICK ATTACK",
    60,
    80,
    0,
};


Move POUND_MOVE = {
    "POUND",
    80,
    40,
    0,
};


Move SHIELD_MOVE = {
    "SHIELD",
    0,
    70,
    100,
};

Move JUMP_MOVE = {
    "JUMP",
    0,
    80,
    90,
};

Move DISSOLVE_MOVE = {
    "DISSOLVE",
    75,
    70,
    0,
};*/
