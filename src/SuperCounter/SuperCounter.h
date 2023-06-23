#ifndef SUPERCOUNTER_H
#define SUPERCOUNTER_H

#include <stdint.h>
#include <avr/interrupt.h>

#define MILLISECONDS_IN_SECOND 1000
#define TIMER0_COMPA_VAL 249


static volatile uint32_t supercounter;
void init_supercounter();

#endif
