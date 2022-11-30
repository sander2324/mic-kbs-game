#include "Nunchuck.h"

#include <Wire.h>

#if NUNCHUCK_DEBUG
#include <HardwareSerial.h>
#endif


NunchuckClass::NunchuckClass() {}


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


void NunchuckClass::get_nunchuck_data_buffer(uint8_t* buffer) {
    Wire.requestFrom(this->twi_address, (uint8_t)NUNCHUCK_BUFFER_LENGTH);
    for (uint8_t i = 0; i < NUNCHUCK_BUFFER_LENGTH && Wire.available(); i++) {
        buffer[i] = Wire.read();
    }

    // uint8_t z_button = (this->buffer[2]>>0) & 1;
    // uint8_t c_button = (this->buffer[2]>>1) & 1;

    this->set_nunchuck_zero(); // Reset nunchuck data after read
}


NunchuckJoystickState NunchuckClass::get_nunchuck_joystick_state(uint8_t x, uint8_t y) {

}


NunchuckState NunchuckClass::get_state() {
    uint8_t data_buffer[NUNCHUCK_BUFFER_LENGTH];
    this->get_nunchuck_data_buffer(data_buffer);

    return NunchuckState {
        this->get_nunchuck_joystick_state(data_buffer[0], data_buffer[1]),      // Joystick state
        true,                                                                   // Z pressed
        true,                                                                   // C pressed
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
