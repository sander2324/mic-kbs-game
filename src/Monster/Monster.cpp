#include "Monster.h"


Monster::Monster(
    const char* name,
    Move* moveset,
    uint8_t* sprite,
    uint16_t* sprite_colors,
    uint8_t max_health
) {
    this->name = name;
    this->moveset = moveset;
    this->sprite = sprite;
    this->max_health = max_health;
    this->current_health = max_health;
}
