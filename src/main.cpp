#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <HardwareSerial.h>

#define NUNCHUCK_TWI_ADDR 0x52

#include "Nunchuck/Nunchuck.h"
#include "Display/Display.h"


void init_registers() {

}


void init_twi() {
    Wire.begin();
}


void initialize() {
    sei();
    Serial.begin(9600);
    init_registers();
    init_twi();
    Display.begin();
    Nunchuck.begin(NUNCHUCK_TWI_ADDR);
}


int main() {
    initialize();

    while(true) {
#if NUNCHUCK_DEBUG
        Nunchuck.print_state();
#endif
    }

    return 0;
}
