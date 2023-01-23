#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>


class Move {
    public:
        Move(const char* name, uint8_t power, uint8_t accuracy, uint8_t defense);
        const char* name;
        uint8_t power;
        uint8_t accuracy;
        uint8_t defense;
};


const Move QUICK_ATTACK_MOVE = Move("QUICK ATK", 15, 90, 0);
const Move POUND_MOVE = Move("POUND", 40, 40, 0);
const Move SHIELD_MOVE = Move("SHIELD", 0, 70, 100);
const Move JUMP_MOVE = Move("JUMP ATK", 20, 85, 0);
const Move DISSOLVE_MOVE = Move("DISSOLVE", 30, 80, 0);
const Move FIRE_BREATH_MOVE = Move("FIRE BREATH", 30, 80, 0);
const Move SCRATCH_MOVE = Move("SCRATCH", 20, 90, 0);
const Move FELINE_FRENZY_MOVE = Move("FELINE FRENZY", 60, 40, 0);
const Move WHISKER_WHIP_MOVE = Move("WHISKER WHIP", 15, 80, 0);
const Move POUNCE_MOVE = Move("POUNCE", 30, 80, 0);

#endif
