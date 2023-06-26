#ifndef MENU_H
#define MENU_H

#include "util/delay.h"

#include "../global_vars.h"
#include "../Display/Display.h"
#include "../Nunchuck/Nunchuck.h"
#include "../Monster/Monster.h"

#include "../sprites/font.h"


class MenuClass {
    public:
        MenuClass();
        void draw_title();
        void draw_character_select(MonsterKind* selected_monster_kind);
};


extern MenuClass Menu;

#endif
