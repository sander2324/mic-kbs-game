#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>


#define NUNCHUCK_TWI_ADDR 0x52

#include "global_vars.h"

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "Monster/Monster.h"
#include "Monster/Move.h"
#include "sprites/font.h"
#include "sprites/test.h"
#include "PersistentStorage/PersistentStorage.h"
#include "Battle/Battle.h"

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif

void init_registers() {

}


void init_twi() {
    Wire.begin();
}


void initialize() {
    sei();
#if NUNCHUCK_DEBUG
    Serial.begin(9600);
#endif
    init_registers();
    init_twi();
    Display.begin();
    PersistentStorage.begin();
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
}


int main() {
    initialize();

    char player_name[PLAYER_NAME_ARRAY_LENGTH];
    PersistentStorage.get_player_name(player_name);
    BattlePlayer user = {
        player_name,
        Monster(MonsterKind::SLIME_MONSTER, SLIME_MOVESET),
        PersistentStorage.get_most_consecutive_wins(),
    };

    BattlePlayer opponent = {
        "OPPONENT",
        Monster(MonsterKind::KITTY_MONSTER, KITTY_MOVESET),
        15
    };

    Battle current_battle = Battle(user, opponent);
    while (true) {
        current_battle.tick();
    }

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
