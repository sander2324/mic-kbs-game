#include "Monster.h"


Monster::Monster(MonsterKind monster_kind) {
    switch (monster_kind) {
        case MonsterKind::SLIME_MONSTER: {
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
