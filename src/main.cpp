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
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
}


int main() {
    init();

    while(true) {
#if NUNCHUCK_DEBUG
        Nunchuck.print_state();
#endif
    }

    return 0;
}
