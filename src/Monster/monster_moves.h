#ifndef MOVES_H
#define MOVES_H

#include <stdint.h>

class Move {
    public:
        Move(const char* name, uint8_t power, uint8_t accuracy, uint8_t defense);
        const char* name;
        uint8_t power;
        uint8_t accuracy;
        uint8_t defense;
};

#endif
