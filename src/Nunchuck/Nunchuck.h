#ifndef NUNCHUCK_H
#define NUNCHUCK_H

#ifndef NUNCHUCK_TWI_ADDR
#define NUNCHUCK_TWI_ADDR 0x52
#endif

#include <stdint.h>


enum NunchuckJoyDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

NunchuckJoyDirection nunchuck_get_current_joy_direction();
void nunchuck_init();

#endif
