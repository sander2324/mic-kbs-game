#include "SuperCounter.h"

volatile uint32_t supercounter = 0;

ISR(TIMER0_COMPA_vect) {
    supercounter += 1;
}


// Set Timer0 to trigger TIMER0_COMPA interrupt every 1 millisecond
void init_supercounter() {
    // Set Timer0 to wave generation mode 2 (CTC)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Set Timer0 prescaler to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02);

    // Set Output Compare Register A
    OCR0A = TIMER0_COMPA_VAL;

    // Clear Timer0
    TCNT0 = 0;

    // Timer0 Output Compare Match A (OCR0A) Interrupt Enable
    TIMSK0 |= (1 << OCIE0A);
}
