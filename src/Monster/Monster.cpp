#include "Monster.h"

Monster::Monster(MonsterKind monster_kind, Move* moveset) {
    switch (monster_kind) {
        case MonsterKind::SLIME_MONSTER: {
            this->name = "SLIME";
            this->moveset = moveset;
            this->sprite = SLIME_SPRITE;
            this->sprite_colors = SLIME_COLORS;
            this->max_health = 100;
            this->current_health = 100;
            break; // Stats
        }
        case MonsterKind::FIRE_SLIME_MONSTER: {
            this->name = "FIRE SLIME";
            this->moveset = moveset;
            this->sprite = SLIME_SPRITE;
            this->sprite_colors = RED_SLIME_COLORS;
            this->max_health = 100;
            this->current_health = 100;
            break; // Stats
        }
        case MonsterKind::KITTY_MONSTER: {
            this->name = "KITTY";
            this->moveset = moveset;
            this->sprite = KITTY_SPRITE;
            this->sprite_colors = KITTY_COLORS;
            this->max_health = 100;
            this->current_health = 100;
            break; // Stats
        }
    }
}


const uint8_t* Monster::get_sprite() {
    switch (this->kind) {
        case MonsterKind::SLIME_MONSTER: {
            return SLIME_SPRITE;
        }
    }
}


const uint16_t* Monster::get_sprite_colors() {
    switch (this->kind) {
        case MonsterKind::SLIME_MONSTER: {
            return SLIME_COLORS;
        }
    }
}
