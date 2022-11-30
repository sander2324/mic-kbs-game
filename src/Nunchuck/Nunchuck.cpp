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
    int8_t buffer;
    Wire.requestFrom(twi_address, 6);
    for (uint8_t i = 0; i < 6 && Wire.available(); i++) {

    }

    // Process buffer.



    uint8_t z_button = (this->buffer[2]>>0) & 1;
    uint8_t c_button = (this->buffer[2]>>1) & 1;

    this->set_nunchuck_zero(); // Reset nunchuck data after read
}


NunchuckState NunchuckClass::get_state() {
    NunchuckState result;
    uint8_t buffer_index = 0;
    uint8_t buffer;
    Wire.requestFrom(twi_address, 6);
    for (uint8_t i = 0; i < 6 && Wire.available(); i++) {
        buffer = Wire.read();
        switch (i)
            {
          case 0: {
            // Process joystick_x data.
            //if (outbuf[0] >= 70||(outbuf[0] < 0 && outbuf[0] <= -120)) {joy_x_axis = 0;} else if (outbuf[0] < 70 && outbuf[0] >= 0) {joy_x_axis = 1;} else {joy_x_axis = -1;} // Process X input data by setting bit.
            if (buffer<70) {
                result.joystick_state = NunchuckJoystickState::LEFT;
            } else if (buffer <= 192) {
                result.joystick_state = NunchuckJoystickState::CENTER;
            } else if (buffer > 192) {
                result.joystick_state = NunchuckJoystickState::RIGHT;
            }
            break;
          }


          case 1: {
            if (buffer<70) {
                result.joystick_state = NunchuckJoystickState::UP;
            } else if (buffer <= 192) {
                // Do nothing, as it's left, right or center.
            } else if (buffer > 192) {
                result.joystick_state = NunchuckJoystickState::DOWN;
            }
              break;
          }

          case 5: {

              break;
          }


          default:

              break;
        }
    }

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
