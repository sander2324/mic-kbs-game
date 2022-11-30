#include "Nunchuck.h"

#include <Wire.h>


void nunchuck_init() {
    Wire.beginTransmission(NUNCHUCK_TWI_ADDR);
    Wire.write(0xF0);
    Wire.write(0x55);
    Wire.endTransmission();
}


NunchuckJoyDirection nunchuck_get_current_joy_direction() {
    return NunchuckJoyDirection::RIGHT;
}
