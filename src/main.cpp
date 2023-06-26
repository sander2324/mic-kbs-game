#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <util/delay.h>

#include <HardwareSerial.h>


#define NUNCHUCK_TWI_ADDR 0x52

#include "global_vars.h"

#include "Infrared/Infrared.h"
#include "SuperCounter/SuperCounter.h"
#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Monster/Monster.h"
#include "Monster/Move.h"
#include "sprites/font.h"
#include "sprites/test.h"
#include "PotMeter/PotMeter.h"
#include "PersistentStorage/PersistentStorage.h"
#include "Battle/Battle.h"

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


inline void init_twi() {
    Wire.begin();
}


void initialize() {
#if NUNCHUCK_DEBUG
    Serial.begin(9600);
#endif
    init_supercounter();
    init_twi();

    sei();

    Display.begin();
    PersistentStorage.begin();
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
    PotMeter.prepareADC();
    Infrared.begin();
}

// ADC done/ready ISR.
ISR(ADC_vect) {
    PotMeter.rawADC = ADC;
    PotMeter.adcReady = true;
}

// Semi-temp ISR loop for brightness controls.
ISR(TIMER2_OVF_vect) {
    PORTD |= (1<<PORTD3);
}

ISR(TIMER2_COMPA_vect) {
    PORTD &= ~(1<<PORTD3);
}


int main() {
    initialize();
    Serial.begin(9600);

    Menu.draw_title();
    MonsterKind player_monster_kind;
    if (Nunchuck.current_state.c_pressed) {
        player_monster_kind = MonsterKind::KITTY_MONSTER;
    } else {
        player_monster_kind = MonsterKind::SLIME_MONSTER;
    }

    srand(supercounter);  // Set random seed

    char player_name[PLAYER_NAME_ARRAY_LENGTH];
    PersistentStorage.get_player_name(player_name);
    BattlePlayer user = {
        player_name,
        Monster(player_monster_kind),
        PersistentStorage.get_most_consecutive_wins(),
    };

    BattlePlayer opponent = {
        "OPPONENT",
        Monster::get_random(),
        15,
    };

    Battle current_battle = Battle(user, opponent);
    while (true) {
        current_battle.loop();
        PotMeter.checkPotmeterBrightness();
    }

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
