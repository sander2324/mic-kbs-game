#include "Infrared.h"


InfraredClass::InfraredClass() {}


InfraredClass Infrared = InfraredClass();


void InfraredClass::begin() {
    DDRD |= (1 << DDD6);  // Set infrared pin to OUTPUT mode


    // Set timer1 to mode 4 (CTC with OCR1A as TOP)
    TCCR1B &= ~(1 << WGM10 | 1 << WGM11);
    TCCR1B |= (1 << WGM12);

    TCNT1 = 0;  // Reset timer1 counter
}


void InfraredClass::send_byte(uint8_t byte) {

}
