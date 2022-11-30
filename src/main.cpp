#include <avr/io.h>
#include <Wire.h>
#include <HardwareSerial.h>

#define NUNCHUCK_TWI_ADDR 0x52

#include "Nunchuck/Nunchuck.h"


void init_registers() {

}


void init_twi() {
    Wire.begin();
}


void init() {
    Serial.begin(9600);
    init_registers();
    init_twi();
}


int main() {
    init();
    Serial.println("Begin");
    Nunchuck nunchuck = Nunchuck();
    Serial.println("End");
    Serial.flush();


    Serial.println(nunchuck.get_current_joy_direction());

    return 0;
}
