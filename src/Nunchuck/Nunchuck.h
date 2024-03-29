#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#define NUNCHUCK_DEBUG 0

#define NUNCHUCK_BUFFER_LENGTH 6

#define NUNCHUCK_Z_BUTTON_SHIFT 0
#define NUNCHUCK_C_BUTTON_SHIFT 1

#define NUNCHUCK_STATE_ADDR 0x00
#define NUNCHUCK_CALIBRATION_ADDR 0x20
#define NUNCHUCK_ID_ADDR 0xFA

#define NUNCHUCK_DEADZONE_START 64
#define NUNCHUCK_DEADZONE_END 192


#include <stdint.h>


enum NunchuckJoystickState {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
};


struct NunchuckState {
    NunchuckJoystickState joystick_state : 3;
    bool z_pressed : 1;
    bool c_pressed : 1;
};


class NunchuckClass {
    public:
        NunchuckClass();
        void begin(uint8_t twi_address);
        void set_nunchuck_zero();
        NunchuckState current_state;
        NunchuckState previous_state;
        void fetch_state();
        bool state_changed();
        void wait_for_z();

    private:
        uint8_t twi_address;
        NunchuckJoystickState get_nunchuck_joystick_state(uint8_t x, uint8_t y);
        void get_nunchuck_data_buffer(uint8_t* buffer);

#if NUNCHUCK_DEBUG
    public:
        void print_state();
        void print_joystick_raw();
        const char* get_joystick_state_screen_name(NunchuckState state);
#endif
};


extern NunchuckClass Nunchuck;

#endif
