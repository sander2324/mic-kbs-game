#include "monster_moves.h"

Move::Move(char* name, uint8_t power, uint8_t accuracy, uint8_t defense) {
    this->name = name;
    this->power = power;
    this->accuracy = accuracy;
    this->defense = defense;
};
