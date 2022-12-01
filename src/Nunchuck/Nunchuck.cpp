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

    this->set_nunchuck_zero(); // Reset nunchuck data after read
}


NunchuckJoystickState NunchuckClass::get_nunchuck_joystick_state(uint8_t x, uint8_t y) {
    if (!((x < NUNCHUCK_DEADZONE_START || x > NUNCHUCK_DEADZONE_END) || (y < NUNCHUCK_DEADZONE_START || y > NUNCHUCK_DEADZONE_END))) {
        return NunchuckJoystickState::CENTER;
    } // Get deadzone'd

    if (x < NUNCHUCK_DEADZONE_START && (y < NUNCHUCK_DEADZONE_END && y > NUNCHUCK_DEADZONE_START)) return NunchuckJoystickState::LEFT;
    else if (x > NUNCHUCK_DEADZONE_END && (y < NUNCHUCK_DEADZONE_END && y > NUNCHUCK_DEADZONE_START)) return NunchuckJoystickState::RIGHT;
    else if (y < NUNCHUCK_DEADZONE_START && (x < NUNCHUCK_DEADZONE_END && x > NUNCHUCK_DEADZONE_START)) return NunchuckJoystickState::DOWN;
    else if (y > NUNCHUCK_DEADZONE_END && (x < NUNCHUCK_DEADZONE_END && x > NUNCHUCK_DEADZONE_START)) return NunchuckJoystickState::UP;
}


void NunchuckClass::set_state() {
    uint8_t data_buffer[NUNCHUCK_BUFFER_LENGTH];
    this->get_nunchuck_data_buffer(data_buffer);

    this->previous_state = this->current_state;
    this->current_state = {
        this->get_nunchuck_joystick_state(data_buffer[0], data_buffer[1]),      // Joystick state
        !(data_buffer[5] >> NUNCHUCK_Z_BUTTON_SHIFT & 1),                       // Z pressed
        !(data_buffer[5] >> NUNCHUCK_C_BUTTON_SHIFT & 1),                       // C pressed
    };
}


bool NunchuckClass::state_changed() {
    return (
        this->current_state.joystick_state != this->previous_state.joystick_state
        || this->current_state.z_pressed != this->previous_state.z_pressed
        || this->current_state.c_pressed != this->previous_state.c_pressed
    );
}


#if NUNCHUCK_DEBUG
const char* NunchuckClass::get_joystick_state_screen_name(NunchuckState state) {
    switch (state.joystick_state) {
        case NunchuckJoystickState::UP: return "UP";
        case NunchuckJoystickState::DOWN: return "DOWN";
        case NunchuckJoystickState::LEFT: return "LEFT";
        case NunchuckJoystickState::RIGHT: return "RIGHT";
        case NunchuckJoystickState::CENTER: return "CENTER";
    }
}


void NunchuckClass::print_state() {
    this->set_state();

    if (!this->state_changed()) return;

    Serial.print("Joystick: ");
    Serial.print(this->get_joystick_state_screen_name(this->current_state));
    Serial.print("\t\t\t");
    Serial.print(this->current_state.joystick_state != NunchuckJoystickState::CENTER ? "\t" : "");
    Serial.print(" | \tC: ");
    Serial.print(this->current_state.c_pressed ? "PRESSED" : "RELEASED");
    Serial.print("\t|\tZ: ");
    Serial.println(this->current_state.z_pressed ? "PRESSED" : "RELEASED");
    Serial.flush();
}


void NunchuckClass::print_joystick_raw() {
    uint8_t data_buffer[NUNCHUCK_BUFFER_LENGTH];
    this->get_nunchuck_data_buffer(data_buffer);
    Serial.print("X: ");
    Serial.println(data_buffer[0]);
    Serial.print("Y: ");
    Serial.println(data_buffer[1]);
}
#endif


NunchuckClass Nunchuck = NunchuckClass();
