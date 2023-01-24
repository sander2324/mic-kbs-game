#ifndef PERSISTENT_STORAGE_H
#define PERSISTENT_STORAGE_H

#include <stdint.h>

#define EEPROM_CELL_INITIAL_VALUE 0

#define PLAYER_NAME_MAX_LENGTH 7

#define EEPROM_INITIALIZED_CHECK_INDEX 0
#define GAME_SETTINGS_INDEX 1
#define PLAYER_NAME_INDEX 5
#define MOST_CONSECUTIVE_WINS_INDEX PLAYER_NAME_INDEX + PLAYER_NAME_MAX_LENGTH

#define PLAYER_NAME_ARRAY_LENGTH PLAYER_NAME_MAX_LENGTH + 1


struct GameSettings {
    bool has_finished_setup : 1;
};


class PersistentStorageClass {
    public:
        void begin();

        GameSettings get_game_settings();
        void set_game_settings(GameSettings settings);

        void get_player_name(char* player_name);
        void set_player_name(const char* name);

        uint16_t get_most_consecutive_wins();
        void set_most_consecutive_wins(uint16_t current_consecutive_wins);

        void reset();

    private:
        GameSettings get_default_game_settings();
        void clear_eeprom();
};


extern PersistentStorageClass PersistentStorage;

#endif
