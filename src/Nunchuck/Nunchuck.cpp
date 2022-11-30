#include "Nunchuck.h"

#include <Wire.h>

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


NunchuckClass::NunchuckClass() {}


uint8_t NunchuckClass::buffer[NUNCHUCK_CHUNK_LENGTH];


void NunchuckClass::begin(uint8_t twi_address) {
    Wire.beginTransmission(twi_address);
    Wire.write(0x40);
    Wire.write(0x00);
    Wire.endTransmission();
}


void NunchuckClass::set_buffer() {
    // TODO
}


NunchuckState NunchuckClass::get_state() {

    return NunchuckState {
        NunchuckJoystickState::UP,      // Joystick state
    };
}


#if NUNCHUCK_DEBUG
const char* get_joystick_state_screen_name(NunchuckState state) {
    switch (state.joystick_state) {
        case NunchuckJoystickState::UP: return "UP";
        case NunchuckJoystickState::DOWN: return "DOWN";
        case NunchuckJoystickState::LEFT: return "LEFT";
        case NunchuckJoystickState::RIGHT: return "RIGHT";
    }
}

void NunchuckClass::print_state() {
    Serial.print("Joystick state: ");
    Serial.println(get_joystick_state_screen_name(this->get_state()));
    Serial.flush();
}
#endif


NunchuckClass Nunchuck = NunchuckClass();
