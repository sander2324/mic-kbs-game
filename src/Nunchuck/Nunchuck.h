#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#define NUNCHUCK_DEBUG 0

#define NUNCHUCK_CHUNK_LENGTH 32

#define NUNCHUCK_STATE_ADDR 0x00
#define NUNCHUCK_CALIBRATION_ADDR 0x20
#define NUNCHUCK_ID_ADDR 0xFA


#include <stdint.h>


enum NunchuckJoystickState {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};


struct NunchuckState {
    NunchuckJoystickState joystick_state;
};


class NunchuckClass {
    public:
        NunchuckClass();
        void begin(uint8_t twi_address);
        NunchuckState get_state();

    private:
        static uint8_t buffer[];
        void set_buffer();

#if NUNCHUCK_DEBUG
    public:
        void print_state();
#endif
};


extern NunchuckClass Nunchuck;

#endif
