#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#define NUNCHUCK_DEBUG 1

#define NUNCHUCK_BUFFER_LENGTH 3

#define NUNCHUCK_STATE_ADDR 0x00
#define NUNCHUCK_CALIBRATION_ADDR 0x20
#define NUNCHUCK_ID_ADDR 0xFA


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
        NunchuckState get_state();

    private:
        uint8_t twi_address;
        static uint8_t buffer[];
        void set_buffer();

#if NUNCHUCK_DEBUG
    public:
        void print_state();
#endif
};


extern NunchuckClass Nunchuck;

#endif
