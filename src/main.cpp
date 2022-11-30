#include <avr/io.h>
#include <avr/interrupt.h>
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
    sei();
    Serial.begin(9600);
    init_registers();
    init_twi();
}


int main() {
    init();
    Serial.println("Begin");
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
    Serial.println("End");
    Serial.flush();


    NunchuckState nunchuck_state = Nunchuck.get_state();

    Serial.println(sizeof(nunchuck_state));

    return 0;
}
