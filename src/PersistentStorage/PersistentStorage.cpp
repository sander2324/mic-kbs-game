#include "PersistentStorage.h"

#include <EEPROM.h>


void PersistentStorageClass::begin() {
    /*
        First index of the EEPROM should always be 0.
        If not, reset storage.
    */
    bool eeprom_is_not_initialized = EEPROM.read(EEPROM_INITIALIZED_CHECK_INDEX);
    if (eeprom_is_not_initialized) {
        this->reset();
    }
}


GameSettings PersistentStorageClass::get_game_settings() {
    GameSettings game_settings_type;
    return EEPROM.get(GAME_SETTINGS_INDEX, game_settings_type);
}


void PersistentStorageClass::set_game_settings(GameSettings settings) {
    EEPROM.put(GAME_SETTINGS_INDEX, settings);
}


void PersistentStorageClass::get_player_name(char* player_name) {
    uint8_t index;
    for (index = 0; index < PLAYER_NAME_MAX_LENGTH; index += 1) {
        const char current_char = EEPROM.read(index + PLAYER_NAME_INDEX);
        if (current_char == EEPROM_CELL_INITIAL_VALUE) {
            break;
        }
        player_name[index] = current_char;
    }

    player_name[index] = '\0';
}


void PersistentStorageClass::set_player_name(const char* name) {
    uint8_t index = 0;

    while (name[index] != '\0' && index < PLAYER_NAME_MAX_LENGTH) {
        EEPROM.update(
            index + PLAYER_NAME_INDEX,
            name[index]
        );
        index += 1;
    }
    while (index < PLAYER_NAME_MAX_LENGTH) {
        EEPROM.update(
            index + PLAYER_NAME_INDEX,
            EEPROM_CELL_INITIAL_VALUE
        );
        index += 1;
    }
}


uint16_t PersistentStorageClass::get_most_consecutive_wins() {
    uint16_t most_consecutive_wins_type;
    return EEPROM.get(MOST_CONSECUTIVE_WINS_INDEX, most_consecutive_wins_type);
}


void PersistentStorageClass::set_most_consecutive_wins(uint16_t current_consecutive_wins) {
    const uint16_t most_consecutive_wins = this->get_most_consecutive_wins();

    if (current_consecutive_wins > most_consecutive_wins) {
        EEPROM.put(MOST_CONSECUTIVE_WINS_INDEX, current_consecutive_wins);
    }
}


void PersistentStorageClass::reset() {
    this->clear_eeprom();
    this->set_game_settings(this->get_default_game_settings());
}


GameSettings PersistentStorageClass::get_default_game_settings() {
    GameSettings default_settings;

    default_settings.has_finished_setup = false;

    return default_settings;
}


void PersistentStorageClass::clear_eeprom() {
    for (uint16_t index = 0; index < EEPROM.length(); index++) {
        EEPROM.update(index, EEPROM_CELL_INITIAL_VALUE);
    }
}


PersistentStorageClass PersistentStorage = PersistentStorageClass();
