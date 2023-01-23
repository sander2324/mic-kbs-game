#include "PotMeter.h"

#ifndef uint8_t
    #include <stdint.h>
#endif


#ifndef DDRB
    #include <avr/io.h>
#endif

#ifndef sei
    #include <avr/interrupt.h>
#endif

PotMeterClass::PotMeterClass() {}

void PotMeterClass::setBacklightPinRaw(int input) {
    if (input==1) {
        DDRD |= (1<<PORTD3);
        PORTD |= (1<<PORTD3);
    } else {
        DDRD |= (1<<PORTD3);
        PORTD &= ~(1<<PORTD3);
    }
    return;
}

void PotMeterClass::setBacklightBrightness(uint8_t inputByte) {
    // Use a 0 - 255 value to control & check the PWM's signal.
    this->setPWM(inputByte,0);
    return;
}

void PotMeterClass::setPWM(uint8_t inputByte, bool reset) {
    // Directly access the TC2 to set up the PWM signal.
    // For this, we need TC2:
    // (WGM) Mode 3,

    // We only need the last 2 bits of this register to be 1.
    TCCR2A |= (1<<WGM20)|(1<<WGM21);
    // The second byte will be written dynamically, due to unused bytes that do not need to be toggled.
    TCCR2B &= ~(1<<WGM22); // Set WGM 22 to 0, now Mode 3 is fully selected.
    TCCR2B |= (1<<CS22);
    TCCR2B &= ~((1<<CS21) | (1<<CS20)); // Select the 64 prescaler, which makes the program a bit more friendly on the CPU, edit this where needed for performance against speed.

    OCR2A = inputByte; // Set input to OCR, which in our case becomes the TOP.
    TIMSK2 |= (1<<TOIE0)|(1<<OCIE2A); // Enable interrupts on TOIE2.

    if (reset==true) {
        TCNT2 = 0;
    }

    sei();

    return;
}

PotMeterClass PotMeter = PotMeterClass();
