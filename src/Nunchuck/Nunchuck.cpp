#include "Nunchuck.h"

#include <Wire.h>

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


NunchuckClass::NunchuckClass() {}


uint8_t NunchuckClass::buffer[NUNCHUCK_BUFFER_LENGTH];


void NunchuckClass::set_nunchuck_zero() {
    Wire.beginTransmission(twi_address);
    Wire.write(0);
    Wire.endTransmission();
}


void NunchuckClass::begin(uint8_t twi_address) {
    this->twi_address = twi_address;

    Wire.beginTransmission(this->twi_address); // Begin transmission with given address via handshake, send command to disable encrypted signal.
    Wire.write(0xF0);
    Wire.write(0x55); // Fill Wire buffer with 0xF0, 0x55
    Wire.endTransmission(); // Send data to Nunchuck.
    Wire.beginTransmission(this->twi_address);
    Wire.write(0xFB);
    Wire.write(0x00);
    Wire.endTransmission();
    this->set_nunchuck_zero();
}


void NunchuckClass::set_buffer() {
    uint8_t buffer_index = 0;
    Wire.requestFrom(twi_address, 6);
    for (uint8_t i = 0; i < 6 && Wire.available(); i++) {
        if (i == 0 || i == 1 || i == 5) {
            this->buffer[buffer_index] = Wire.read();
            buffer_index += 1;
        } else {
            Wire.read();
        }
    }

    // Process buffer.



    uint8_t z_button = (this->buffer[2]>>0) & 1;
    uint8_t c_button = (this->buffer[2]>>1) & 1;

    this->set_nunchuck_zero(); // Reset nunchuck data after read
}


NunchuckState NunchuckClass::get_state() {
    this->set_buffer();


    return NunchuckState {
        NunchuckJoystickState::UP,      // Joystick state
        true,                           // Z pressed
        true,                           // C pressed
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
    Serial.println(sizeof(this->get_state()));
    Serial.flush();
}
#endif


NunchuckClass Nunchuck = NunchuckClass();
