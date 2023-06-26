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
        current_battle.loop();

        // Temporary insert for change brightness.
        PotMeter.checkPotmeterBrightness();
        // Remove the lines above once main is reformatted to have a solid loop part.
    }

#if NUNCHUCK_DEBUG
    while(true) {
        Nunchuck.print_state();
    }
#endif

    return 0;
}
