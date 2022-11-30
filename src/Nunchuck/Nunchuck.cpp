#include "Nunchuck.h"

#include <Wire.h>


Nunchuck::Nunchuck() {
    this->init();
}


void Nunchuck::init() {
    Wire.beginTransmission(NUNCHUCK_TWI_ADDR);
    Wire.write(0xF0);
    Wire.write(0x55);
    Wire.endTransmission();
}


NunchuckJoyDirection Nunchuck::get_current_joy_direction() {
    return NunchuckJoyDirection::RIGHT;
}
